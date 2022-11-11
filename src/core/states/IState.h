#pragma once

#include "../systems/input/InputSystem.h"
#include "../systems/video/VideoSystem.h"

class IState {
private:
	std::shared_ptr<InputSystem> inputSystem;
	std::shared_ptr<VideoSystem> videoSystem;

public:
	IState(
		std::shared_ptr<InputSystem>& inputSystem,
		std::shared_ptr<VideoSystem>& videoSystem
	) : inputSystem(inputSystem), videoSystem(videoSystem) { }

	virtual void handleInput() = 0;
	virtual void update() = 0;
	virtual void render() = 0;
};