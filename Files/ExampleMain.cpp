// This is ExampleMain.cpp. This file is where you adjust rewards, weights, hyperparameters and everything that makes your bot play like a bot.
// These are your imports. In hindsight, this calls the file and connects it to here where you can adjust your features!

#include <GigaLearnCPP/Learner.h>
#include <RLGymCPP/Rewards/CommonRewards.h>
#include <RLGymCPP/Rewards/ZeroSumReward.h>
#include <RLGymCPP/TerminalConditions/NoTouchCondition.h>
#include <RLGymCPP/TerminalConditions/GoalScoreCondition.h>
#include <RLGymCPP/OBSBuilders/DefaultObs.h>
#include <RLGymCPP/OBSBuilders/AdvancedObs.h>
#include <RLGymCPP/StateSetters/KickoffState.h>
#include <RLGymCPP/StateSetters/RandomState.h>
#include <RLGymCPP/ActionParsers/DefaultAction.h>

using namespace GGL;
using namespace RLGC;

// Here's where your rewards go. Your rewards live in a file called "CommonRewards.h", and they use math to reward the bot for succeeding in doing an action that the code relates to.
// You have your reward name (e.g VelocityPlayerToBallReward), and your weight (1.f). But why are some weighed higher than others? The reason being is because:
// If I weighed FaceBallReward higher than TouchBallReward, PPO would think facing the ball is better than touching it. Then it wouldn't learn to touch very much, if not at all.
// Have a go at using these to begin with. Once your bot learns to move efficiently, uncomment the other two rewards in there and see what happens!

EnvCreateResult EnvCreateFunc(int index) {
    std::vector<WeightedReward> rewards = {
      //  { new GoalReward(-.80f), 150.f },
      //  { new VelocityBallToGoalReward(), 5.f },
        { new VelocityPlayerToBallReward(), 1.f },
        { new FaceBallReward(), .1f },
        { new TouchBallReward(), 5.f },
        { new AirReward(), .1f },
    };

    std::vector<TerminalCondition*> terminalConditions = {
        new NoTouchCondition(10), // "NoTouchCondition(10)" states that if you don't touch the ball for 10 seconds, the environment resets back to its initial phase.
        new GoalScoreCondition()
    };

// This is where you set your arena up. Right now, we can see that this is a 1v1-only environment in Soccar. It's like this because the "playersPerTeam" value is set to "1".

    int playersPerTeam = 1; // If you set this to "int playersPerTeam = 2;", that would make a 2v2 environment. Same as ""int playersPerTeam = 3;", that's a 3v3 environment.
    auto arena = Arena::Create(GameMode::SOCCAR);
    for (int i = 0; i < playersPerTeam; i++) {
        arena->AddCar(Team::BLUE);
        arena->AddCar(Team::ORANGE);
    }

// Here's the "EnvCreateResult" mentioned in the repo. Right now, we have "DefaultAction" as our action parser, "AdvancedObs" as our observation builder and "KickoffState" as the state.

    EnvCreateResult result = {};
    result.actionParser = new DefaultAction(); // "DefaultAction" is the action parser where your bot selects buttons to move, jump, throttle, boost and more.
    result.obsBuilder = new AdvancedObs(); // "AdvancedObs" gives structured physics info (positions, velocities, rotations) unlike DefaultObs.
    result.stateSetter = new KickoffState(); // "KickoffState" is the state used to make kickoffs the only state that happens at the start of the environment.
    result.terminalConditions = terminalConditions;
    result.rewards = rewards;
    result.arena = arena;

    return result;
}

// Not too important for beginners. It just records simple stats about players and the ball during the game.
// Optional diagnostics. This does NOT affect training, only logging.

void StepCallback(Learner* learner, const std::vector<GameState>& states, Report& report) {
    bool doExpensiveMetrics = (rand() % 4) == 0;

    for (auto& state : states) {
        if (doExpensiveMetrics) {
            for (auto& player : state.players) {
                report.AddAvg("Player/In Air Ratio", !player.isOnGround);
                report.AddAvg("Player/Ball Touch Ratio", player.ballTouchedStep);
                report.AddAvg("Player/Demoed Ratio", player.isDemoed);
                report.AddAvg("Player/Speed", player.vel.Length());

                Vec dirToBall = (state.ball.pos - player.pos).Normalized();
                report.AddAvg("Player/Speed Towards Ball", RS_MAX(0, player.vel.Dot(dirToBall)));
                report.AddAvg("Player/Boost", player.boost);

                if (player.ballTouchedStep)
                    report.AddAvg("Player/Touch Height", state.ball.pos.z);
            }
        }

        if (state.goalScored)
            report.AddAvg("Game/Goal Speed", state.ball.vel.Length());
    }
}

int main(int argc, char* argv[]) {
    RocketSim::Init("C:/Users/file_location"); // Change this line to wherever your "collision_meshes" file lives. Otherwise, GigaLearnBot.exe won't launch and will spam errors.

    LearnerConfig cfg = {};
    cfg.deviceType = LearnerDeviceType::GPU_CUDA; // Use GPU_CUDA for GPU training. CPU_CUDA runs on CPU but still uses CUDA-backed ops.
    cfg.tickSkip = 8; // How many physics ticks happen per agent decision.
    cfg.actionDelay = cfg.tickSkip - 1;
    cfg.numGames = 256; // How many games run at once.

    int tsPerItr = 50000; // Total no. of timesteps collected every PPO iteration.
    cfg.ppo.tsPerItr = tsPerItr;
    cfg.ppo.batchSize = tsPerItr;
    cfg.ppo.miniBatchSize = 50000; // Size of each minibatch during training.
    cfg.ppo.epochs = 1; // Number of times PPO re-trains on the same data.
    cfg.ppo.entropyScale = 0.045f; // Encourages randomness in actions. Higher = more exploration. Lower = less exploration.
    cfg.ppo.gaeGamma = 0.99f; // How far it sees into the future.

    // Policy = Decides what action to take. Critic = Decides how good the situation is.
    // Too high = unstable learning. Too low = Polishing too early, or not enough learning.
    cfg.ppo.policyLR = 2e-4;
    cfg.ppo.criticLR = 2e-4;

    cfg.ppo.sharedHead.layerSizes = { 512, 512, 256 }; // Used by both policy and critic layer sizes; extract more common features.
    cfg.ppo.policy.layerSizes = { 512, 512, 256 }; // Size of policy (higher = more ceiling to get better but slower. Lower = opposite).
    cfg.ppo.critic.layerSizes = { 512, 512, 256 }; // Size of critic (same rule as above).

    auto optim = ModelOptimType::ADAM;
    cfg.ppo.policy.optimType = optim;
    cfg.ppo.critic.optimType = optim;
    cfg.ppo.sharedHead.optimType = optim;

    auto activation = ModelActivationType::LEAKY_RELU; // Keep it as is; prevents dead neurons.
    cfg.ppo.policy.activationType = activation;
    cfg.ppo.critic.activationType = activation;
    cfg.ppo.sharedHead.activationType = activation;

    bool addLayerNorm = true;
    cfg.ppo.policy.addLayerNorm = addLayerNorm;
    cfg.ppo.critic.addLayerNorm = addLayerNorm;
    cfg.ppo.sharedHead.addLayerNorm = addLayerNorm;

    cfg.trainAgainstOldVersions = false; // Set this to true to train against old versions.
    cfg.trainAgainstOldChance = 0.15f; // Self explanatory. 15% environments train against old versions. 85% train against same model.

    // WandB Stats! If you have a WandB account, this shows your rating (not always accurate though)!

    cfg.skillTracker.enabled = true;
    cfg.skillTracker.numArenas = 16;
    cfg.skillTracker.simTime = 45;
    cfg.skillTracker.updateInterval = 16;
    cfg.skillTracker.ratingInc = 5;
    cfg.skillTracker.initialRating = 0;

    cfg.sendMetrics = false; // If you have a WandB account, set this to true.
    cfg.renderMode = false;

    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--render") {
            cfg.renderMode = true;
            cfg.renderTimeScale = 1.5f; // Speed of your visualiser.
            cfg.sendMetrics = false;
            cfg.ppo.miniBatchSize = 0;
            cfg.numGames = 1;
            cfg.ppo.deterministic = false; // Make this "true" if you want to see your bot play with its best decisions (so far).
            break;
        }
    }

    // If it reaches here, your environment/learner is successful!

    Learner* learner = new Learner(EnvCreateFunc, cfg, StepCallback);
    learner->Start();

    return EXIT_SUCCESS;
}

// Now, a few more things. If there's lines that are blank or don't have comments about them, you don't need to worry too much about them.
// This repo and ExampleMain.cpp are meant for people who don't understand GigaLearn too well. I'm a beginner too. I've studied the framework more than I've made a bot myself.
// I'm absolutely not your go-to for making a bot long term, but I know the project inside out. I could tell you almost every part of it. BUT, that doesn't mean I can fix everything.
// I am in a sketchy server; it's where a lot of the cheaters are. If you want to ask a question, I have a thread in EvilRLgym under "public-content". I don't cheat myself, though.
// Credit where it's due; check out Zealan's GitHub for the project and Kue's GitHub for more information; not just mine. This same comment is in my GitHub, but enjoy coding!
