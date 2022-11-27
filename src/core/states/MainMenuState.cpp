#include "MainMenuState.h"

MainMenuState::MainMenuState(
	std::shared_ptr<InputSystem>& inputSystem,
	std::shared_ptr<VideoSystem>& videoSystem,
	std::shared_ptr<PhysicsSystem>& physicsSystem
) : IState(inputSystem, videoSystem, physicsSystem) {
	auto frames = this->videoSystem->getTextureManager()->getTexturesFromSpriteSheet(
		Paths::CHARACTERS_DIR + "rogue.png",
		{10, 10, 10, 10}
	);

	this->test_t = frames[0][0];

	this->test = std::make_shared<AnimationView>(frames[0]);
}	

void MainMenuState::handleInput() {}

void MainMenuState::update(float dTime) {
	this->test->update(dTime);
};

void MainMenuState::render() {
	this->videoSystem->draw(test);
};