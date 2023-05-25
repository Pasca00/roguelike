#pragma once

#include "Entity.h"

class Player : public Entity {
private:


public:
	Player(
		std::shared_ptr<Movable>& movableComponent,
		std::vector<std::shared_ptr<AnimationView>>& idleAnimations,
		std::vector<std::shared_ptr<AnimationView>>& attackAnimations,
		std::vector<std::shared_ptr<AnimationView>>& moveAnimations,
		std::shared_ptr<AnimationView>& deadAnimation
	);

	void handleInput(std::shared_ptr<Input>& input);
	void update(float dtime);
	std::shared_ptr<AnimationView> getCurrentTexture();

	static const Uint8 STATE_IDLE		= 0;
	static const Uint8 STATE_WALKING	= 1;
	static const Uint8 STATE_ATTACKING	= 2;
	static const Uint8 STATE_DEAD		= 3;
};