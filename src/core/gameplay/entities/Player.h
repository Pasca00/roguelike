#pragma once

#include "../../systems/physics/Movable.h"
#include "PlayerStates.h"

class Player {
private:
	std::shared_ptr<Movable> movable;

	std::unique_ptr<PlayerIdleState> idleState;
	std::unique_ptr<PlayerWalkingState> walkingState;
	std::unique_ptr<PlayerAttackState> attackState;
	std::unique_ptr<PlayerDyingState> dyingState;

	Entity* currentState;

	//Uint8 currentState;

public:
	Player(
		glm::vec2& position,
		std::shared_ptr<AnimationView>& idleAnimation1,
		std::shared_ptr<AnimationView>& idleAnimation2,
		std::shared_ptr<AnimationView>& attackAnimation,
		std::shared_ptr<AnimationView>& walkingAnimation,
		std::shared_ptr<AnimationView>& deathAnimation
	);

	void handleInput(std::shared_ptr<Input>& input);
	void update(float dtime);
	std::shared_ptr<AnimationView> getCurrentTexture();

	static const Uint8 STATE_IDLE		= 0;
	static const Uint8 STATE_WALKING	= 1;
	static const Uint8 STATE_ATTACKING	= 2;
	static const Uint8 STATE_DEAD		= 3;
};