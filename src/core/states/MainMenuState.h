#pragma once

#include "IState.h"

class MainMenuState : public IState {
private:

public:
	MainMenuState(
		std::shared_ptr<InputSystem>& inputSystem,
		std::shared_ptr<VideoSystem>& videoSystem,
		std::shared_ptr<PhysicsSystem>& physicsSystem
	);

	void handleInput() override;
	void update(float dTime) override;
	void render() override;
};