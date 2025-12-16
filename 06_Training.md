# 06: Letting Your Bot Train

## How do I start training?

Now that you've (hopefully) finished with `ExampleMain.cpp` and your settings, you're ready to train.

### Step 1: Compile
Build the project in Visual Studio 2022 (or your IDE that's compatible). Make sure it compiles without errors. 

### Step 2: Run
Run the compiled executable. You'll see output in the console showing:
- Games starting
- Timesteps being collected
- Iterations completing
- Metrics being logged

### Step 3: Monitor
If WandB is enabled, open your WandB dashboard to watch metrics in real-time. When you launch GigaLearnBot.exe, you'll be given a link. Hold Shift + Click to open it on your browser.

## What happens during my training?

Your bot:
1. Plays games in parallel (256 games by default)
2. Collects experience (observations, actions, rewards)
3. Updates its neural network using PPO (the learning algorithm)
4. Saves checkpoints periodically
5. Repeats

Each iteration collects 50,000 timesteps (by default), then updates the policy.

## How long does training take?

Depending on your CPU/GPU, training can take **BILLIONS of timesteps** to produce a decent bot.

Speed depends on your hardware. A good GPU can hit 100,000+ steps per second (SPS) on ~2-3 million parameters.

## Monitoring Progress

Watch these metrics in WandB:
- **Policy Loss:** Should decrease early, then stabilize
- **Value Loss:** Should decrease over time
- **Entropy:** Should slowly decrease (bot becoming more confident)
- **Rewards/GoalReward (WandB):** Should increase as bot learns to score
- **Player/Ball Touch Ratio:** Should increase as bot learns to hit the ball

If metrics stay flat or rewards don't improve after hundreds of millions of steps, something might be wrong (bad rewards, bad learning rate, etc.).

## Checkpoints

GigaLearn saves checkpoints every 1 million steps (by default) to the `checkpoints/` folder.

Each checkpoint includes:
- Policy network weights
- Optimizer state
- Metrics and logs

You can resume training from any checkpoint if your run crashes.

## Stopping Training

Press `Q` during training to save and quit cleanly. If you kill the process, you'll only lose a million steps; it's fine.

## Common Issues

### Why's my bot not learning / why is it regressing (getting worse)?
- Check WandB – are rewards increasing?
- Is your entropyScale too high?
- Did you use/spam AI rewards that kill your bot or teach it bad behaviour?
- Is your learning rate too high?
- Are your tickSkip/gaeGamma in line with each other?

It'll eventually stop learning, anyway. The parameters will be holding what they can; you'll be very far into training and it'll naturally slow down.

### Why's my training so slow?
- Lower `cfg.numGames` if you're running out of RAM
- Lower `cfg.ppo.miniBatchSize` if you're running out of VRAM
- Running too many parallel environments; GPU/CPU can't handle it
- Parameters are too big
- Make sure you're using GPU, not CPU; CPU is far slower

### Why is my bot doing weird things?
- Check your rewards – are you over-rewarding or under-rewarding something?
- Look at raw reward logs in WandB
- Try reducing entropy scale if bot is too random or playing unsmoothly

## Key Points

- Training takes billions of timesteps to produce a good bot
- Monitor progress in WandB (policy loss, rewards, metrics)
- Save checkpoints regularly (happens automatically)
- Press `Q` to quit cleanly and save
- Adjust learning config if training is too slow or bot isn't improving
