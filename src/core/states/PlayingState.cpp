#include "PlayingState.h"

PlayingState::PlayingState(
	bool& stateChange,
	std::shared_ptr<InputSystem>& inputSystem,
	std::shared_ptr<VideoSystem>& videoSystem,
	std::shared_ptr<PhysicsSystem>& physicsSystem,
	std::shared_ptr<SoundSystem>& soundSystem,
	std::shared_ptr<GeneralSystem>& generalSystem
) : IState(stateChange, inputSystem, videoSystem, physicsSystem, soundSystem, generalSystem) {
	{
		auto castleStage = std::make_shared<CastleStage>(
			std::static_pointer_cast<IGenerator>(std::make_shared<BSPGenerator>(50, 100, 5)),
			this->videoSystem->getTextureManager(),
			32
		);

		this->levelManager = std::make_unique<LevelManager>(
			castleStage,
			16
		);

		castleStage->generateStage();
	}

	{
		this->soundSystem->loadMusic(Paths::AMBIENCE_DIR + "castle_ambience.mp3", "castle");
		this->soundSystem->playMusic("castle");
	}

	this->videoSystem->endTransition();
}

PlayingState::~PlayingState() {}

void PlayingState::handleInput() {

}

void PlayingState::update(float dTime) {

}

void PlayingState::render() {
	auto tileMap = this->levelManager->getTileMap();

	for (int i = 0; i < tileMap.size(); i++) {
		for (int j = 0; j < tileMap[i].size(); j++) {
			if (tileMap[i][j] != nullptr) {
				this->videoSystem->draw(std::static_pointer_cast<IView>(tileMap[i][j]->getView()));
			}
		}
	}
}