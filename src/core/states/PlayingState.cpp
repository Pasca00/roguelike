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
		this->soundSystem->loadMusic(Paths::MUSIC_DIR + "dark_forest.mp3", "castle");
	}

	{
		auto textureManager = this->videoSystem->getTextureManager();
		auto rogueTextures = textureManager->getTexturesFromSpriteSheet(
			Paths::CHARACTERS_DIR + "rogue.png",
			{
				10, // death
				10, // attack
				10, // walk
				10, // idle1
				10, // idle2
			}
		);

		auto playerHitbox = std::make_shared<Hitbox>(30, 30);
		auto idle1 = std::make_shared<AnimationView>(rogueTextures[0], false, 0.15f, 120, 120, 3);
		auto idle2 = std::make_shared<AnimationView>(rogueTextures[1], false, 0.15f, 120, 120, 3);
		auto walk = std::make_shared<AnimationView>(rogueTextures[2], true, 0.15f, 120, 120, 3);
		auto attack = std::make_shared<AnimationView>(rogueTextures[3], false, 0.15f, 120, 120, 3);
		auto death = std::make_shared<AnimationView>(rogueTextures[4], false, 0.15f, 120, 120, 3);

		this->player = std::make_unique<Player>(
			glm::vec2(30, 30),
			idle1,
			idle2,
			attack,
			walk,
			death
		);
	}

	this->videoSystem->endTransition();
}

PlayingState::~PlayingState() {}

void PlayingState::handleInput() {
	auto input = this->inputSystem->getInput();
	
	this->player->handleInput(input);
}

void PlayingState::update(float dTime) {
	this->player->update(dTime);
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


	this->videoSystem->draw(std::static_pointer_cast<IView>(this->player->getCurrentTexture()));
}

void PlayingState::executePostLoad() {
	this->soundSystem->setMusicLevel(8);
	this->soundSystem->playMusic("castle");
}