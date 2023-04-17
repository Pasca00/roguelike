#pragma once

#include "../../systems/input/InputSystem.h"
#include "../../systems/video/views/AnimationView.h"

class Entity {
protected:
	std::shared_ptr<AnimationView> animation;

public:
	Entity(std::shared_ptr<AnimationView>& animation);

	virtual void handleInput(std::shared_ptr<Input>& input) = 0;
	virtual void update(float dtime) = 0;

	std::shared_ptr<AnimationView> getCurrentTexture();
};