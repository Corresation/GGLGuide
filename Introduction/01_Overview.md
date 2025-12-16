# 01: Overview and Introduction

## What is GigaLearnCPP?

GigaLearn is a fast training system for Rocket League bots. It uses PPO. It runs alongside/with RocketSim. It teaches a bot how to play by giving it rewards.
Simple as that if you dumb it down.

## What is "PPO"?

PPO is a training method. 
The bot tries actions, gets rewards and learns which actions are good. 
It repeats this many times and improves gradually. 
This depends on your structure, though.

## So, how does training work?

1. The bot sees the game state.
2. The bot chooses an action.
3. The bot gets a reward, or doesn't and keeps trying.
4. The bot updates its brain.
5. This repeats for millions and billions of steps.

## What can I control when my bot is training?

You control:

* Rewards
* Observations
* Config settings
* State setters
* How long it trains

## GigaLearn is the quickest public framework, however it was leaked by a group of cheaters

It runs in C++. It uses high speed physics and collects a lot of steps per second from the efficient environment. 
This makes gathering timesteps much quicker than other environments. However, it was unfortunately given out to the public for people to exploit. Considering this cannot be undone, I believed it would be helpful to get people started using this framework since it's inevitable.

## NEXT SECTION

Move to the next section when you understand what GigaLearn is and what training means.
