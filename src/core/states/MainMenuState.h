#pragma once

#include "IState.h"
#include "../systems/video/views/AnimationView.h"

class MainMenuState : public IState {
private:
	std::shared_ptr<AnimationView> test;
	std::shared_ptr<Texture> test_t;
	
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