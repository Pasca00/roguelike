#include "PlayingState.h"

PlayingState::PlayingState(
	bool& stateChange,
	std::shared_ptr<InputSystem>& inputSystem,
	std::shared_ptr<VideoSystem>& videoSystem,
	std::shared_ptr<PhysicsSystem>& physicsSystem,
	std::shared_ptr<SoundSystem>& soundSystem,
	std::shared_ptr<GeneralSystem>& generalSystem
) : IState(stateChange, inputSystem, videoSystem, physicsSystem, soundSystem, generalSystem) {
	this->levelManager = std::make_unique<LevelManager>(
		std::make_unique<BSPGenerator>(50, 100, 5)
	);

	this->levelManager->generateTemplate();
}

PlayingState::~PlayingState() {}

void PlayingState::handleInput() {

}

void PlayingState::update(float dTime) {

}

void PlayingState::render() {

}