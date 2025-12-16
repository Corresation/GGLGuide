# 05: State Setters

## What are State Setters?

A **StateSetter** controls how each game starts when it resets. Does the game begin at kickoff? Does it start with random car and ball positions? Does the ball or the player spawn mid-air, or on the ground?

This determines what situations your bot practices.

## Why do they even matter?

If you only train on kickoffs, your bot will only learn kickoffs. If you want it to handle air dribbles, you need to put it in those situations.

State setters define the "curriculum" your bot learns from.

## Common State Setters

### KickoffState
Games start at kickoff (standard spawn positions, ball at center).

```cpp
result.stateSetter = new KickoffState();
```

**Use this when:** You want your bot to practice kickoffs and standard gameplay.

### RandomState
Cars and ball spawn in completely random positions and velocities.

```cpp
result.stateSetter = new RandomState(true, true, false);
```

**Use this when:** You want your bot to practice recovery, aerials, and unusual situations.

### CombinedState
Combines multiple state setters with weights. Randomly picks one each reset.

```cpp
result.stateSetter = new CombinedState({
    {new KickoffState(), 0.7f},      // 70% kickoffs
    {new RandomState(true, true, false), 0.3f}  // 30% random mid-air
});
```

**Use this when:** You want variety. Your bot practices multiple scenarios in one run.

## Which StateSetters should I use?

For beginners:
- Start with **KickoffState** – Simple, consistent, teaches basic gameplay.
- Once your bot can hit the ball reliably, add **RandomState** or **CombinedState** to teach adaptability.

For advanced training:
- Use **CombinedState** with multiple setters to expose your bot to diverse scenarios.

## How do I change it?

In `ExampleMain.cpp`, look for this line:
```cpp
result.stateSetter = new KickoffState();
```

Replace it with whichever state setter you want.

There are many other StateSetters that can be created. You can make a FlickState, AirDribbleState or others. People tend to do this with AI, though.

## Key Points

- State setters control how games start
- KickoffState = standard kickoff spawns
- RandomState = random positions and velocities
- FuzzedKickoffState = kickoff with tiny variations
- CombinedState = mix multiple setters with weights
- Choose based on what situations you want your bot to practice
