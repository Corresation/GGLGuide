# 03: Rewards

## What Are Rewards?

Rewards tell your bot what's good and what's bad. When your bot does something you want (like touching the ball), it gets a positive reward. When it does something you don't want (like getting scored on), it will get a negative reward.

Rewards are how you teach your bot what the goal is.

## How Rewards Work

Every frame, your bot gets a reward value (a number). Positive = good. Negative = bad. Zero = neutral.

For example (this is using my default ExampleMain.cpp as reference):
- Touch the ball → +5.0 reward
- Score a goal → +150 reward
- Get scored on → -120 reward

Your bot tries to collect as much reward as possible over time. Sometimes, the bot can compete to get more than the opponent!

## Important: Timesteps Teach Most Things

Here's what beginners get wrong: **you don't need rewards for everything.**

The bot learns most things naturally through experience (called "timesteps"). It will learn:
- Defense
- Rotations
- Wall play
- Accuracy
- When to save boost
- How to recover

You only need rewards for:
- **Core actions:** Touching the ball, facing it, moving toward it
- **Specific mechanics:** Wavedashes, air dribbles, flip resets (things it won't discover on its own)
- **Big events:** Goals, demos, saves

## Common Rewards

Here are the basic rewards you can always use, and it's self explanatory:

### Basic Rewards
- `GoalReward` – Rewards the bot for scoring against the opponent
- `TouchBallReward` – Rewards touching the ball
- `VelocityPlayerToBallReward` – Rewards moving toward the ball
- `VelocityBallToGoalReward` – Rewards ball moving toward opponent goal
- `PickupBoostReward` – Rewards grabbing boost pads
- `SaveBoostReward` – Rewards having boost (encourages conservation)
- `FaceBallReward` – Rewards facing the ball
- `AirReward` – Rewards being in the air (prevents forgetting)

## Zero-Sum Rewards

Remember before where I mentioned the bot could compete for rewards? A **ZeroSumReward** wraps another reward and makes it competitive. For example, when you score, you get +reward. When the opponent scores, you get -reward.

Example:
```cpp
{ new ZeroSumReward(new VelocityBallToGoalReward(), 0, 1), 5.0f }
```

The `0` is **teamSpirit**:
- `0` = You get all the reward (no sharing with teammates)
- `1` = Reward is split equally with teammates

The `1` is **opponentScale**:
- `1` = Full zero-sum (opponent's reward subtracts from yours)
- `0` = Not zero-sum (opponent's reward doesn't affect you)

The `5.0f` is the **weight** (how important this reward is).

## Reward Weights

Weights control how much each reward matters. Higher weight = more important.

**Do NOT guess at weights unless you're experimenting for fun. Or... if you somehow get lucky.** Start with proven values from examples. Adjust slowly if needed.

Example reward stack from `ExampleMain.cpp`:
```cpp
        { new VelocityPlayerToBallReward(), 1.f },
        { new FaceBallReward(), .1f },
        { new TouchBallReward(), 5.f },
        { new AirReward(), .1f },
```

## Key Points

- Rewards teach your bot what to do
- Most things are learned naturally through timesteps (not rewards)
- Only use rewards for core actions, specific mechanics, and major events
- Start with proven reward stacks, don't invent your own weights until confident
- ZeroSumReward makes training competitive between teams
