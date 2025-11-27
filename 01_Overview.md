# OVERVIEW

This section explains the basics of GigaLearn in simple terms.

## What IS GigaLearn?

GigaLearn is a fast training system for Rocket League bots. It uses PPO. It runs alongside/with RocketSim. It teaches a bot how to play by giving it rewards.
Simple as that if you dumb it down.

## WHAT PPO MEANS

PPO is a training method. 
The bot tries actions, gets rewards and learns which actions are good. 
It repeats this many times and improves gradually. 
This depends on your structure, though.

## HOW TRAINING WORKS

1. The bot sees the game state.
2. The bot chooses an action.
3. The bot gets a reward, or doesn't and keeps trying.
4. The bot updates its brain.
5. This repeats for millions and billions of steps.

## WHAT YOU CAN CONTROL

You control:

* Rewards
* Observations
* Config settings
* State setters
* How long it trains

## WHY GGL IS ONE OF THE QUICKEST

It runs in C++. It uses high speed physics and collects a lot of steps per second from the efficient environment. 
This makes gathering timesteps much quicker than other environments.

## NEXT SECTION

Move to the next section when you understand what GigaLearn is and what training means.
