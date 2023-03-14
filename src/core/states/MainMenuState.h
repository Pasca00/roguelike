#pragma once

#include "IState.h"
#include "../systems/video/views/AnimationView.h"
#include "../systems/video/views/TextView.h"

class MainMenuState : public IState {
private:
	std::unordered_map<std::string, std::shared_ptr<IView>> views;

	std::shared_ptr<TextView> textView;
	
public:
	MainMenuState(
		std::shared_ptr<InputSystem>& inputSystem,
		std::shared_ptr<VideoSystem>& videoSystem,
		std::shared_ptr<PhysicsSystem>& physicsSystem,
		std::shared_ptr<SoundSystem>& soundSystem
	);

	void handleInput() override;
	void update(float dTime) override;
	void render() override;
};