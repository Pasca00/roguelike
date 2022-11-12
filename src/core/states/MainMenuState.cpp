#include "MainMenuState.h"

MainMenuState::MainMenuState(
	std::shared_ptr<InputSystem>& inputSystem,
	std::shared_ptr<VideoSystem>& videoSystem
) : IState(inputSystem, videoSystem) {
	

}	

void MainMenuState::handleInput() {}

void MainMenuState::update() {};

void MainMenuState::render() {};