#include "MainMenuState.h"

MainMenuState::MainMenuState(
	std::shared_ptr<InputSystem>& inputSystem,
	std::shared_ptr<VideoSystem>& videoSystem,
	std::shared_ptr<PhysicsSystem>& physicsSystem
) : IState(inputSystem, videoSystem, physicsSystem) {
	auto textureManager = this->videoSystem->getTextureManager();
	auto frames = textureManager->getTexturesFromSpriteSheet(
		Paths::CHARACTERS_DIR + "rogue.png",
		{10, 10, 10, 10, 10}
	);
	
	this->views["idle"] = std::static_pointer_cast<IView>(std::make_shared<AnimationView>(frames[0]));
	this->views["idle2"] = std::static_pointer_cast<IView>(std::make_shared<AnimationView>(frames[1]));
	this->views["running"] = std::static_pointer_cast<IView>(std::make_shared<AnimationView>(frames[2]));
	this->views["attack"] = std::static_pointer_cast<IView>(std::make_shared<AnimationView>(frames[3]));
	this->views["death"] = std::static_pointer_cast<IView>(std::make_shared<AnimationView>(frames[4]));

	this->videoSystem->initFramebuffer();

	auto titlescreen = textureManager->getSingleTextureFromFile(Paths::CHARACTERS_DIR + "../titlescreen.png");
	this->views["titlescreen"] = std::static_pointer_cast<IView>(
		std::make_shared<View>(titlescreen, 0, 0, 5)
	);
}

void MainMenuState::handleInput() {
	auto input = this->inputSystem->getInput();
}

void MainMenuState::update(float dTime) {
	this->views["idle"]->update(dTime);
};

void MainMenuState::render() {
	this->videoSystem->bindFrameBuffer();
	this->videoSystem->draw(this->views["titlescreen"]);
	this->videoSystem->draw(this->views["idle"]);
	this->videoSystem->unbindFramebuffer();

	this->videoSystem->drawFrameBuffer("rain", physicsSystem->getTotalTime());
};