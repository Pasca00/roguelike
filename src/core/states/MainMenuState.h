#pragma once

#include "IState.h"
#include "../systems/video/views/AnimationView.h"

class MainMenuState : public IState {
private:
	std::unordered_map<std::string, std::shared_ptr<IView>> views;
	
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