# Tips & Troubleshooting

This is a big list of advice for making a successful beginner bot. These are **behaviour problems**, not setup errors. If your training isn't even starting, that's a different issue—this guide assumes you have GigaLearn already running.

---

## The most common excuses:

### "My bot just spins in circles / does nothing..."
- **Cause:** Rewards aren't clear enough or are conflicting.
- **Fix:** Start with the simplest reward stacks (TouchBallReward, FaceBallReward, VelocityToBallReward).
- **Fix:** Make sure rewards like FaceBallReward aren't too high and aren't bigger than your main event rewards.
- **Fix:** Check if entropy is too high (bot is too random). Gradual decay is a brilliant practice (decrease entropyScale over a long period of time).

### "My bot ball-chases but never rotates back!"
- This is normal early on. **Hell, it's normal for billions of steps.** Rotation is learned through timesteps, not rewards.
- Don't add rotation rewards. Let it learn naturally. If it's really ballchasing, again, consider reward weights.

### "The bot isn't even jumping, it's just always on the ground."
- **Cause:** Not rewarded for being in the air.
- **Fix:** Add `AirReward` with a small weight (0.1-0.15 ideally).

### "My bot isn't even getting any boost."
- **Cause:** This is a frequent issue. Despite PickupBoostReward being high, you have to introduce it at the right time. Too late means the policy prioritises other things. Too early means the bot will just love boost too much.
- **Fix:** Observe your WandB metrics properly (related rewards) and the visualiser to determine the right time to introduce it.

---

## Training Speed Issues

### Training is really slow (under 10,000 SPS).
- **Check GPU usage:** Make sure you're using `GPU_CUDA`, not CPU.
- **Lower `cfg.numGames`:** If you're running out of RAM, reduce games.
- **Lower `cfg.ppo.miniBatchSize`:** If you're running out of VRAM, reduce this (50,000 -> 25,000 is typical and perfectly fine!)
- **Check tick skip:** Higher tick skip = faster training but less precise control (8 is standard, 4 is for advanced movement).

### SPS drops over time during training
- **This is normal.** Neural network gets bigger in memory as it learns
- **Fix:** Restart training every 500M-1B steps if it drops significantly
- **Fix:** Lower `miniBatchSize` if VRAM is maxing out

### Training stutters or freezes periodically
- **Cause:** Checkpoint saving
- **Fix:** Increase `cfg.tsPerSave` to save less frequently (e.g., 5M instead of 1M)
- **Cause:** WandB logging overhead
- **Fix:** Set `cfg.sendMetrics = false` temporarily to test if WandB is the issue

---

## Reward Problems

### Rewards aren't increasing at all
- **Check WandB:** Are rewards flat at zero? Bot might not be reaching reward conditions (mainly an AI issue from vibe-coding).
- **Fix:** Make sure terminal conditions aren't resetting games too quickly (increase `NoTouchCondition` time).

### One reward dominates everything (huge positive/negative values in WandB)
- **Cause:** The reward weight is far too high.
- **Fix:** Simply lower the weight of the reward; it's easy.

### Bot learns one thing but forgets another
- **Cause:** Catastrophic forgetting (one reward overpowers others too much, but not enough to disrupt training properly).
- **Fix:** Balance reward weights more carefully and be more patient with training. Let it train more before flooding rewards.
- **Fix:** Lower learning rate to slow down forgetting.
- **Fix:** Use `trainAgainstOldVersions` to maintain old skills.

---

## Common Beginner Mistakes

### Changing too many things at once
- **Problem:** You change rewards, learning rate, tick skip, and network layers all in one run
- **Result:** You don't know what caused the improvement or regression
- **Fix:** Change **one thing at a time** and train for 50M+ steps to see the effect.
NOTE: Sometimes this works in the framework. You can spam rewards earlier on, get better speed but general results for rotation will flatline. It can still end up SCARILY good if done right.

### Switching observation builders mid-training.
- **Problem:** You start with `DefaultObs` then switch to `AdvancedObs`.
- **Result:** Training crashes.
- **Fix:** **Never change obs during training.** Pick one and stick with it for the entire run.

### Over-rewarding mechanics too early
- **Problem:** You add flip reset rewards, air dribble rewards, etc. at 1B steps
- **Fix:** Bot needs to learn basics first (touch, face, move toward ball)
- **Fix:** Add advanced mechanic rewards later, man.

### Not letting timesteps do their job
- **Problem:** You add rewards for rotation, defense, backboard saves, etc.
- **Fix:** Timesteps teach these naturally. Don't over-reward everything
- **Fix:** Only reward core actions (touch, face, boost) and major events (goals, demos)
NOTE: It can work; it can boost training time, but only if you have the right reward. It's very pointless, though. It usually intervenes and create noise.

### Giving up too early
- **Problem:** Bot doesn't score goals consistently by 100M steps, so you restart
- **Fix:** Bots take 50-100M steps to start scoring consistently
- **Fix:** Be patient. Training is slow.

### Copying reward weights from someone else's bot without understanding them.
- **Problem:** You see a 1v1 bot with `VelocityBallToGoalReward` at 10.0 and copy it.
- **Fix:** Weights are context-dependent. Start with proven defaults, adjust slowly.
- **Fix:** Understand what each reward does before changing weights.

---

## What's Normal vs. What's Broken

### Normal things that look broken:
- **Bot doesn't score until 50M+ steps** → This is fine
- **Rewards increase slowly** → Learning is gradual
- **Bot keeps flipping after hundreds of millions of steps** → Self-corrects over time. It's VERY normal.
- **Policy loss spikes occasionally** → Normal variance
- **Entropy decreases slowly** → Bot is gaining confidence
- **Bot ball-chases early on** → Rotation comes later
- **WandB rating charts spike and die** → Literally normal as anything. It's only concerning if it's a *BIG* difference.

### Actually broken things:
- **Rewards stay at exactly zero for a long time** → Something is wrong with rewards, or timing isn't right.
- **Entropy drops to near-zero within few steps** → Bot stopped exploring, restart. Don't know how this'd happen, but it has once or twice.
- **SPS drops from 50K to 5K** → Memory leak or VRAM issue. Check if you're playing games while training; is it changing the speed? Did it go OOM (Out Of Memory)?

---

## Using AI to Help You (Claude, ChatGPT, etc.)

### What AI is good at:
- Explaining how rewards work.
- Debugging reward logic (e.g., "Why is my bot doing X?")
- Writing simple custom rewards (basic mechanics).
- Explaining PPO concepts in simple terms.

### What AI is bad at:
- **Weighing rewards.** AI will guess. Don't trust weight suggestions blindly.
- **Creating complex custom rewards.** AI often makes rewards that don't work in practice.
- **Predicting exact training times.** Every setup is different.
- **Knowing your specific bot's behavior without WandB data.** You need to describe symptoms clearly.

### How to get good help from AI:
1. **Be specific.** Don't say: "my bot isn't learning". Say: "My bot isn't defending at all. It's been 5 billion steps; here's my CommonRewards.h file and my current rewards. Why's this happening?"
2. **Share images of your WandB metrics.** Tell AI what your policy loss, entropy, and rewards look like.
3. **Ask one question at a time.** Don't dump 10 problems at once.
4. **Test suggestions one at a time.** Don't change everything AI suggests in one run.

### Red flags when using AI:
- **AI invents stupid reward names** (e.g., "DirectionalMultiAirControlReward") <--- Basically a sign of cluelessness.
- **AI suggests specific weights without knowing your setup** (e.g., "Set VelocityBallToGoalReward to 15")
- **AI suggests changing hyperparameters without explanation.**
- **AI suggests doing impossible things.**
- **AI suggests adding 10+ new rewards at once or changing all of your weights.**

### How to verify AI suggestions:
- Cross-check reward names with `CommonRewards.h`
- Check if suggested hyperparameters are in reasonable ranges (LR: 1e-5 to 3e-4, entropy: 0.03-0.05)
- Ask AI to explain *why* it's suggesting something before implementing
- Test changes for 50M+ steps before judging if they worked
- Add some files of mistakes you've made. Open a project, put it in there and you'll gain better results the more you fail and realise.

---

## Final, genuine advice:

### Have fun with it.
- Training RL bots is trial and error.
- Enjoy being a beginner. Once you start progressing, it can easily become boring or uninteresting; being clueless is so much more entertaining than knowing it all.
- Don't stress if it's slow. Honestly; everyone's first bots are either skidded (that's just boring) or they don't work. It's not bad that your bot is failing, just... keep going.
If you're a cheater, whatever. You're your own person. Have fun using your spare time screwing up a game for temporary satisfaction. Not even sure how people get dopamine by consistently doing this.
