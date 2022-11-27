#include "MainMenuState.h"

MainMenuState::MainMenuState(
	std::shared_ptr<InputSystem>& inputSystem,
	std::shared_ptr<VideoSystem>& videoSystem,
	std::shared_ptr<PhysicsSystem>& physicsSystem
) : IState(inputSystem, videoSystem, physicsSystem) {
	
}	

void MainMenuState::handleInput() {}

void MainMenuState::update(float dTime) {
	printf("%f\n", dTime);
};

void MainMenuState::render() {};