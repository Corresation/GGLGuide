# 04: ExampleMain.cpp

## What is ExampleMain.cpp?

`ExampleMain.cpp` is the most important file in your entire training setup. It's where everything begins.

This single file controls:
- What rewards your bot gets
- What observations it sees
- How games start (kickoff, random, etc.)
- What mode you're playing
- Learning rates, batch sizes, tick skip (TL;DR how unpolished, polished or fast your bot trains)
- WandB setup (this is in my example connected to this section!)
- Network architecture (how big your layers are)
- When training stops

There are MANY more features to add which are located in:

- GigaLearnCPP/src/public/GigaLearnCPP/LearnerConfig.h
- GigaLearnCPP/src/public/GigaLearnCPP/PPO/PPOLearnerConfig.h

But that comes later! Think of it as the "master config file" but in C++ code form.

## Where is my ExampleMain.cpp located?

You'll (usually) find it at:
```
GigaLearnCPP-Leak-main/src/ExampleMain.cpp
```

This is the file you compile and run to start training.

## What's inside the file, then?

The file has two main parts:

### 1. EnvCreateFunc
This function creates the training environment. It defines:
- **Rewards** – What behaviors get rewarded
- **Terminal conditions** – When a game should reset (goal scored, 10 seconds no touch, etc.)
- **ObsBuilder** – What observations the bot sees
- **StateSetter** – How games start (kickoff, random spawn, etc.)
- **ActionParser** – How the bot's actions are interpreted
- **Arena setup** – Number of players per team, game mode

### 2. main()
This is where you:
- Point to your RocketSim collision meshes
- Set learning config (learning rate, batch size, epochs, etc.)
- Choose device (GPU or CPU)
- Set tick skip and action delay
- Configure WandB configss
- Start the training loop

## Why This File Matters

Every time you want to change something about your bot, you edit this file:
- Want different rewards? Edit the `rewards` vector.
- Want a different observation builder? Change `result.obsBuilder`.
- Want more games running at once? Adjust `cfg.numGames`.
- Want to train longer per iteration? Change `cfg.ppo.tsPerItr`.

## The 0-Step Starting Point

The current version in the repo is a **0-step bot** – meaning it's set up for someone with zero AI training experience. It uses:
- `AdvancedObs` (good default observations)
- `KickoffState` (games start at kickoff)
- A basic but functional reward stack
- Proven hyperparameters that work

You can adjust things slightly, but don't change everything at once. Experiment carefully.

## Key Points

- ExampleMain.cpp is where all training starts
- It controls rewards, observations, game rules, and learning settings
- Found at `GigaLearnCPP-Leak-main/src/ExampleMain.cpp`
- The repo version is a working starting point for beginners
- Edit this file to customize your bot's training

Again, you don't NEED to memorise all of these key terms. It gets easier the more you do this.
