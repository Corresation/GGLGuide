# Reward Progression Guide

This is a quick summary of what I personally did to make a decent bot that plays over billions of steps.
Have a go at replicating this yourself first, and after you can do your own things:

## Step 1: 0 Steps (Starting Point)

Start with the absolute basics. These rewards teach the bot to interact with the ball.

### Rewards:
```cpp
        { new FaceBallReward(), 0.1f },
        { new VelocityPlayerToBallReward(), 1.0f },
        { new TouchBallReward(), 5.0f },
        { new AirReward(), 0.1f },
```

### Hyperparameters:
```cpp
cfg.ppo.entropyScale = 0.045f;
cfg.ppo.gaeGamma = 0.99f;
cfg.ppo.policyLR = 2e-4;
cfg.ppo.criticLR = 2e-4;
```

## Step 2: Introduce GoalReward and VelocityBallToGoalReward

Now that your bot can touch the ball with intent, teach it about goals.

### Add these rewards:
```cpp
        { new GoalReward(-0.80f), 150.0f },
        { new VelocityBallToGoalReward(), 5.0f },
```

### Adjust hyperparameters:
```cpp
cfg.ppo.entropyScale = 0.04f;
cfg.ppo.gaeGamma = 0.99f;
cfg.ppo.policyLR = 1.5e-4;
cfg.ppo.criticLR = 1.5e-4;
```

## Step 3: Kickoff regression; it's normal (optional stage).

As your bot learns to play, it might start ignoring kickoffs or playing them poorly. Fix this with a kickoff reward.

### Add this reward:
```cpp
        { new KickoffProximityReward(), 2.0f },
```

You can still introduce them later if you want, though. Up to you.

## Step 4: You're in the billions.

Now your bot is competent at basic gameplay. You need to infer that before deciding to add this. Introduce boost management.

### Add these rewards:
```cpp
        { new PickupBoostReward(), 50.0f },
        { new SaveBoostReward(), 0.5f },
```
You can differ your rewards here. Put it at 30.f, 40.f, anything that's pretty high up.
Your SaveBoostReward can be slightly lower or higher; it's up to you, again. It won't hurt it drastically unless it's an extreme change.

## Step 5: Enable "trainAgainstOldVersions"

Once your bot is stable and competent, enable training against old checkpoints to prevent skill regression.

### Add these settings:
```cpp
cfg.trainAgainstOldVersions = true;
cfg.oldVersionProbability = 0.15f;
```
Your bot now plays 15% against old versions, and the rest is still against itself. Cool.

## Step 6: Dribbling & Flicking are emerging

Your bot is now showing signs of independent dribbling and flicking (usually 3B+ steps). Make the game competitive with zero-sum rewards.

### Replace `VelocityBallToGoalReward` with zero-sum version:
```cpp
        { new ZeroSumReward(new VelocityBallToGoalReward(), 0, 1), 5.0f },
```

This is an ideal start up to a few billion steps.

I'm not going to coach through everything as self-discovery is the important part of learning. After this, I would recommend letting it train for a while.

You can boost it with flick and dribble rewards, that's okay.

You can also start giving it rewards for higher jumping/aerials. That's a good start.

Don't rush it. Let it get good at the basics, then start adding what's better for it in the long-term.

## Full Example

Here's what a complete reward stack might look like after following this progression:

```cpp
std::vector<WeightedReward> rewards = {
        // Basics
        { new FaceBallReward(), 0.1f },
        { new VelocityPlayerToBallReward(), 1.0f },
        { new TouchBallReward(), 5.0f },
        { new AirReward(), 0.1f },

        // Goals
        { new GoalReward(-0.80f), 150.0f },
        { new ZeroSumReward(new VelocityBallToGoalReward(), 0, 1), 5.0f },

        // Kickoff
        { new KickoffProximityReward(), 2.0f },

        // Boost
        { new PickupBoostReward(), 50.0f },
        { new SaveBoostReward(), 0.5f },
};
```

**With hyperparameters:**
```cpp
cfg.ppo.entropyScale = 0.035f;   // or 0.03
cfg.ppo.gaeGamma = 0.99f;
cfg.ppo.policyLR = 1e-4;
cfg.ppo.criticLR = 1e-4;

cfg.trainAgainstOldVersions = true;
cfg.oldVersionProbability = 0.15f;
```

Again, this is simply a guide on a basic bot. You can train it to become advanced, but there's not enough to it for it to become amazing.
At best, this will end up competing well with Necto or Nexto. Add your own rewards when the time's right.
```

This is a solid foundation for a competent 1v1 bot trained over 3-5 billion steps.
