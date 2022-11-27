#pragma once

#include "../systems/input/InputSystem.h"
#include "../systems/video/VideoSystem.h"
#include "../systems/physics/PhysicsSystem.h"

class IState {
private:
	std::shared_ptr<InputSystem> inputSystem;
	std::shared_ptr<VideoSystem> videoSystem;
	std::shared_ptr<PhysicsSystem> physicsSystem;

public:
	IState(
		std::shared_ptr<InputSystem>& inputSystem,
		std::shared_ptr<VideoSystem>& videoSystem,
		std::shared_ptr<PhysicsSystem>& physicsSystem
	) : inputSystem(inputSystem), videoSystem(videoSystem), physicsSystem(physicsSystem) { }

	virtual void handleInput() = 0;
	virtual void update(float dTime) = 0;
	virtual void render() = 0;
};