#include "MainMenuState.h"

MainMenuState::MainMenuState(
	std::shared_ptr<InputSystem>& inputSystem,
	std::shared_ptr<VideoSystem>& videoSystem,
	std::shared_ptr<PhysicsSystem>& physicsSystem,
	std::shared_ptr<SoundSystem>& soundSystem
) : IState(inputSystem, videoSystem, physicsSystem, soundSystem) {
	auto textureManager = this->videoSystem->getTextureManager();

	this->videoSystem->initFramebuffer();

	auto titlescreen = textureManager->getSingleTextureFromFile(Paths::CHARACTERS_DIR + "../titlescreen.png");
	this->views["titlescreen"] = std::static_pointer_cast<IView>(
		std::make_shared<View>(titlescreen, 0, 0, 5)
	);

	{
		std::string play = "PRESS ENTER TO START";
		glm::vec2 textDims = this->videoSystem->getCenteredTextPosition(play);
		this->textView = std::make_shared<TextView>(play, textDims.x, textDims.y);
	}

	{
		this->soundSystem->loadMusic(Paths::AMBIENCE_DIR + "rain.mp3", "rain");
		this->soundSystem->playMusic("rain");

		this->soundSystem->loadSound(Paths::SOUNDS_DIR + "/fx/thump.mp3", "thump");
	}

	{
		auto callback = std::make_unique<IInteractable>(
			"ENTER",
			[this]() {
				this->videoSystem->beginTransition();
				this->soundSystem->playSound("thump");
			}
		);
		this->inputSystem->addEventCallback(callback);
	}
}

void MainMenuState::handleInput() {
	auto input = this->inputSystem->getInput();
}

void MainMenuState::update(float dTime) {

};

void MainMenuState::render() {
	this->videoSystem->bindFrameBuffer();

	this->videoSystem->draw(this->views["titlescreen"]);

	this->videoSystem->unbindFramebuffer();

	this->videoSystem->setUintUniform("time", this->physicsSystem->getTotalTime());
	this->videoSystem->drawFrameBuffer("rain");

	this->videoSystem->drawText(this->textView);
};