#include "PlayingState.h"

PlayingState::PlayingState(
	bool& stateChange,
	std::shared_ptr<InputSystem>& inputSystem,
	std::shared_ptr<VideoSystem>& videoSystem,
	std::shared_ptr<PhysicsSystem>& physicsSystem,
	std::shared_ptr<SoundSystem>& soundSystem,
	std::shared_ptr<GeneralSystem>& generalSystem
) : IState(stateChange, inputSystem, videoSystem, physicsSystem, soundSystem, generalSystem) {
	this->soundSystem->stopMusic();
}

PlayingState::~PlayingState() {}

void PlayingState::handleInput() {

}

void PlayingState::update(float dTime) {

}

void PlayingState::render() {

}