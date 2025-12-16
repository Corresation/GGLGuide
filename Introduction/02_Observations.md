# 02: Observations

## What are observations?

Observations (also known as "Obs") are its surroundings your bot sees every frame. They describe the game state: where the ball is, where your car is, where teammates are, how much boost you have, and so on.

Think of it like this: you watch the screen with your eyes. Your bot "watches" with numbers.

In extent to this, your bot can only learn from what it sees. If you don't include certain features in your observations, it could be more difficult for the bot to learn those things. Overdoing an Obs causes slower training and potentially hazardous progression.

**Rule:** The observation itself must stay the same throughout training. You can't switch halfway through.

## What's in an observation?

Every observation is a list of numbers (called a "vector"). Here's what AdvancedObs includes:

- Ball position, velocity, and spin
- Your car's position, velocity, rotation, and boost
- Teammate and opponent positions and velocities
- Boost pad states (available or not)
- Your previous action (what buttons it pressed last frame, or frames before that)

All positions and velocities are **normalized** (shrunk down to smaller numbers) so the bot can learn faster.

## Observation Builders

An **ObsBuilder** is the code that creates these numbers. GigaLearn has a few already built-in options:

- **DefaultObs**: Basic, works fine
- **AdvancedObs**: Better, includes more info like local ball position (where the ball is relative to your car)

You pick one in `ExampleMain.cpp` like this:

```cpp
result.obsBuilder = new AdvancedObs();
```

## Key Points

- Observations are just numbers describing the game
- Your bot learns from what it sees, so include important info
- The observation size must stay constant during training
- AdvancedObs is a good starting point for beginners
