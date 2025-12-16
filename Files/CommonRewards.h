#pragma once
#include "Reward.h"
#include "../Math.h"

namespace RLGC {

	class GoalReward : public Reward {
	public:
		float concedeScale;
		GoalReward(float concedeScale = -1) : concedeScale(concedeScale) {}

		virtual float GetReward(const Player& player, const GameState& state, bool isFinal) {
			if (!state.goalScored)
				return 0;

			bool scored = (player.team != RS_TEAM_FROM_Y(state.ball.pos.y));
			return scored ? 1 : concedeScale;
		}
	};

	class TouchBallReward : public Reward {
	public:
		virtual float GetReward(const Player& player, const GameState& state, bool isFinal) {
			return player.ballTouchedStep;
		}
	};

	class FaceBallReward : public Reward {
	public:
		virtual float GetReward(const Player& player, const GameState& state, bool isFinal) {
			Vec dirToBall = (state.ball.pos - player.pos).Normalized();
			return player.rotMat.forward.Dot(dirToBall);
		}
	};

	class AirReward : public Reward {
	public:
		virtual float GetReward(const Player& player, const GameState& state, bool isFinal) {
			return !player.isOnGround;
		}
	};

	class VelocityPlayerToBallReward : public Reward {
	public:
		virtual float GetReward(const Player& player, const GameState& state, bool isFinal) {
			Vec dirToBall = (state.ball.pos - player.pos).Normalized();
			Vec normVel = player.vel / CommonValues::CAR_MAX_SPEED;
			return dirToBall.Dot(normVel);
		}
	};

	class VelocityBallToGoalReward : public Reward {
	public:
		bool ownGoal = false;
		VelocityBallToGoalReward(bool ownGoal = false) : ownGoal(ownGoal) {}

		virtual float GetReward(const Player& player, const GameState& state, bool isFinal) {
			bool targetOrangeGoal = player.team == Team::BLUE;
			if (ownGoal)
				targetOrangeGoal = !targetOrangeGoal;

			Vec targetPos = targetOrangeGoal ? CommonValues::ORANGE_GOAL_BACK : CommonValues::BLUE_GOAL_BACK;
			Vec ballDirToGoal = (targetPos - state.ball.pos).Normalized();
			return ballDirToGoal.Dot(state.ball.vel / CommonValues::BALL_MAX_SPEED);
		}
	};

	class KickoffProximityReward : public Reward {
	public:
		virtual float GetReward(const Player& player, const GameState& state, bool isFinal) {
			if (std::abs(state.ball.pos.x) > 1.0f || std::abs(state.ball.pos.y) > 1.0f)
				return 0.0f;

			float playerDistToBall = (player.pos - state.ball.pos).Length();
			float closestOpponentDist = FLT_MAX;
			bool foundOpponent = false;

			for (const Player& opponent : state.players) {
				if (opponent.team == player.team || opponent.carId == player.carId)
					continue;

				foundOpponent = true;
				float opponentDistToBall = (opponent.pos - state.ball.pos).Length();

				if (opponentDistToBall < closestOpponentDist)
					closestOpponentDist = opponentDistToBall;
			}

			if (!foundOpponent)
				return 0.0f;

			return (playerDistToBall < closestOpponentDist) ? 1.0f : -1.0f;
		}
	};

	class JumpTouchReward : public Reward {
	public:
		float minHeight, maxHeight, range;

		JumpTouchReward(float minHeight = 92.75f, float maxHeight = 204.0f)
			: minHeight(minHeight), maxHeight(maxHeight) {
			range = maxHeight - minHeight;
		}

		virtual float GetReward(const Player& player, const GameState& state, bool isFinal) {
			if (player.ballTouchedStep && !player.isOnGround && state.ball.pos.z >= minHeight)
				return (state.ball.pos.z - minHeight) / range;

			return 0;
		}
	};

	class DribbleReward : public Reward {
	public:
		float minBallHeight, maxBallHeight, maxDistance, coeff;

		DribbleReward(float minBallHeight = 109.0f, float maxBallHeight = 180.0f,
			float maxDistance = 197.0f, float coeff = 2.0f)
			: minBallHeight(minBallHeight), maxBallHeight(maxBallHeight),
			maxDistance(maxDistance), coeff(coeff) {
		}

		virtual float GetReward(const Player& player, const GameState& state, bool isFinal) {
			if (!player.isOnGround)
				return 0.0f;
			if (state.ball.pos.z < minBallHeight || state.ball.pos.z > maxBallHeight)
				return 0.0f;
			if ((player.pos - state.ball.pos).Length() >= maxDistance)
				return 0.0f;

			float playerSpeed = player.vel.Length();
			float ballSpeed = state.ball.vel.Length();
			float playerSpeedNormalized = playerSpeed / CommonValues::CAR_MAX_SPEED;
			float inverseDifference = 1.0f - std::abs(playerSpeed - ballSpeed);
			float twoSum = playerSpeed + ballSpeed;

			if (twoSum == 0.0f)
				return 0.0f;

			float speedReward = playerSpeedNormalized + coeff * (inverseDifference / twoSum);
			return speedReward;
		}
	};

	class FlickReward : public Reward {
	public:
		float minFlickSpeed, maxFlickHeight;

		FlickReward(float minFlickSpeed = 800.0f, float maxFlickHeight = 300.0f)
			: minFlickSpeed(minFlickSpeed), maxFlickHeight(maxFlickHeight) {
		}

		virtual float GetReward(const Player& player, const GameState& state, bool isFinal) {
			if (!state.prev || !player.ballTouchedStep)
				return 0.0f;

			bool ballWasLow = state.prev->ball.pos.z < maxFlickHeight;
			bool ballNowFast = state.ball.vel.Length() > minFlickSpeed;
			bool ballGoingUp = state.ball.vel.z > 200.0f;

			float prevDistance = (player.pos - state.prev->ball.pos).Length();
			bool playerWasClose = prevDistance < 200.0f;

			if (ballWasLow && ballNowFast && ballGoingUp && playerWasClose) {
				float speedRatio = RS_MIN(1.0f, state.ball.vel.Length() / 2000.0f);
				return speedRatio;
			}

			return 0.0f;
		}
	};

	class SaveBoostReward : public Reward {
	public:
		float exponent;
		SaveBoostReward(float exponent = 0.5f) : exponent(exponent) {}

		virtual float GetReward(const Player& player, const GameState& state, bool isFinal) {
			return RS_CLAMP(powf(player.boost / 100, exponent), 0, 1);
		}
	};

	class PickupBoostReward : public Reward {
	public:
		virtual float GetReward(const Player& player, const GameState& state, bool isFinal) {
			if (!player.prev)
				return 0;

			if (player.boost > player.prev->boost)
				return sqrtf(player.boost / 100.f) - sqrtf(player.prev->boost / 100.f);

			return 0;
		}
	};

	class TeamSpacingReward : public Reward {
	public:
		float minSpacing;

		TeamSpacingReward(float minSpacing = 1500.0f) : minSpacing(minSpacing) {}

		virtual float GetReward(const Player& player, const GameState& state, bool isFinal) override {
			float totalReward = 0.0f;
			int teammateCount = 0;

			for (const Player& teammate : state.players) {
				if (teammate.carId == player.carId || teammate.team != player.team)
					continue;

				teammateCount++;
				float distance = (player.pos - teammate.pos).Length();

				if (distance >= minSpacing) {
					totalReward += 1.0f;
				}
				else {
					float ratio = distance / minSpacing;
					totalReward += ratio;
				}
			}

			return teammateCount > 0 ? totalReward / teammateCount : 0.0f;
		}
	};

	typedef PlayerDataEventReward<&PlayerEventState::goal, false> PlayerGoalReward;
	typedef PlayerDataEventReward<&PlayerEventState::assist, false> AssistReward;
	typedef PlayerDataEventReward<&PlayerEventState::shot, false> ShotReward;
	typedef PlayerDataEventReward<&PlayerEventState::shotPass, false> ShotPassReward;
	typedef PlayerDataEventReward<&PlayerEventState::save, false> SaveReward;
	typedef PlayerDataEventReward<&PlayerEventState::bump, false> BumpReward;
	typedef PlayerDataEventReward<&PlayerEventState::bumped, true> BumpedPenalty;
	typedef PlayerDataEventReward<&PlayerEventState::demo, false> DemoReward;
	typedef PlayerDataEventReward<&PlayerEventState::demoed, true> DemoedPenalty;
}
