#pragma once

#include "../../systems/input/InputSystem.h"
#include "../../systems/video/views/AnimationView.h"
#include "../../systems/physics/Movable.h"

enum class EntityState {
	IDLE, ATTACK, MOVE, DEAD
};

class Entity {
protected:
	std::shared_ptr<Movable> movableComponent;

	EntityState currentState;

	std::shared_ptr<AnimationView> animation;

	std::vector<std::shared_ptr<AnimationView>> idleAnimations;
	std::vector<std::shared_ptr<AnimationView>> attackAnimations;
	std::vector<std::shared_ptr<AnimationView>> moveAnimations;
	std::shared_ptr<AnimationView> deadAnimation;

	void switchState(EntityState to);

public:
	Entity( // TODO: change these to pairs or maps
		std::shared_ptr<Movable>& movableComponent,
		std::vector<std::shared_ptr<AnimationView>>& idleAnimations,
		std::vector<std::shared_ptr<AnimationView>>& attackAnimations,
		std::vector<std::shared_ptr<AnimationView>>& moveAnimations,
		std::shared_ptr<AnimationView>& deadAnimation
	);

	virtual void handleInput(std::shared_ptr<Input>& input) = 0;
	virtual void update(float dtime) = 0;

	std::shared_ptr<AnimationView> getCurrentTexture();
};