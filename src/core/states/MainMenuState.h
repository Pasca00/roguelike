#pragma once

#include "IState.h"

class MainMenuState : public IState {
private:

public:
	MainMenuState(
		std::shared_ptr<InputSystem>& inputSystem,
		std::shared_ptr<VideoSystem>& videoSyste
	);

	void handleInput() override;
	void update() override;
	void render() override;
};