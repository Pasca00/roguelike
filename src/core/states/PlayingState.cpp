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
		this->updateBarrier = std::make_unique<Barrier>(4);
	}

	{
		int tileSize = 64;

		auto castleStage = std::make_shared<CastleStage>(
			std::static_pointer_cast<IGenerator>(std::make_shared<BSPGenerator>(50, 100, 5)),
			this->videoSystem->getTextureManager(),
			tileSize
		);

		this->levelManager = std::make_unique<LevelManager>(
			castleStage,
			tileSize
		);

		castleStage->generateStage();

		auto tileMap = this->levelManager->getTileMap();

		this->physicsSystem->setMap(tileMap, castleStage->getMapTree());
	}

	{
		this->soundSystem->loadMusic(Paths::MUSIC_DIR + "dark_forest.mp3", "castle");
	}

	{
		float playerX = this->levelManager->getCurrentStage()->playerStartPosX;
		float playerY = this->levelManager->getCurrentStage()->playerStartPosY;

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

		auto idle1 = std::make_shared<AnimationView>(rogueTextures[0], false, 0.15f, 120, 120, 3);
		auto idle2 = std::make_shared<AnimationView>(rogueTextures[1], false, 0.15f, 120, 120, 3);
		auto walk = std::make_shared<AnimationView>(rogueTextures[2], true, 0.15f, 120, 120, 3);
		auto attack = std::make_shared<AnimationView>(rogueTextures[3], false, 0.15f, 120, 120, 3);
		auto death = std::make_shared<AnimationView>(rogueTextures[4], false, 0.15f, 120, 120, 3);
		auto playerHitbox = std::make_shared<Hitbox>(playerX, playerY, 40, 40);

		std::vector<std::shared_ptr<AnimationView>> idleVec = { idle1, idle2 };
		std::vector<std::shared_ptr<AnimationView>> walkVec = { walk };
		std::vector<std::shared_ptr<AnimationView>> attackVec = { attack };

		auto movable = std::make_shared<Movable>(playerHitbox, 150.f);

		auto controllableParams = std::make_shared<ControllableParameters>(
			this->physicsSystem->getTimeModifier(),
			movable->maxSpeed,
			movable->doesCollide
		);

		this->player = std::make_unique<Player>(
			movable,
			idleVec,
			attackVec,
			walkVec,
			death,
			controllableParams
		);

		this->physicsSystem->addMovable(movable);

		this->videoSystem->setCameraSubject(playerHitbox);
		this->videoSystem->drawRelativeToCamera(true);
	}

	this->physicsSystem->addMovable(this->videoSystem->getCamera()->getFocusZone());

	this->videoSystem->endTransition();

	this->makeEnemies();
}

PlayingState::~PlayingState() {}

void PlayingState::handleInput() {
	auto input = this->inputSystem->getInput();
	
	this->player->handleInput(input);

	this->enemyTest->feedInputToEntity();
}

void PlayingState::update(float dTime) {
	this->generalSystem->queueThreadJob([&]() {
		auto p = player->getMovableComponent();
		levelManager->updateScoreGrid(dTime, p->hitbox->x, p->hitbox->y);

		updateBarrier->wait();
	});
	
	this->generalSystem->queueThreadJob([&]() {
		this->physicsSystem->update(dTime);
		
		updateBarrier->wait();
	});

	this->generalSystem->queueThreadJob([&]() {
		levelManager->getCurrentStage()->update(dTime);

		updateBarrier->wait();
	});

	this->updateBarrier->wait();

	this->videoSystem->updateCamera(dTime);

	this->player->update(dTime);

	this->enemyTest->update(dTime);
}

void PlayingState::render() {
	auto tileMap = this->levelManager->getTileMap();

	for (int i = 0; i < tileMap.size(); i++) {
		for (int j = 0; j < tileMap[i].size(); j++) {
			if (tileMap[i][j] != nullptr) {
				this->videoSystem->draw(std::static_pointer_cast<IView>(tileMap[i][j]->getView()));

				auto decoration = tileMap[i][j]->getDecoration();
				if (decoration != nullptr) {
					this->videoSystem->draw(decoration);
				}
			}
		}
	}

	this->videoSystem->draw(std::static_pointer_cast<IView>(this->enemyTest->getEntity()->getCurrentTexture()));
	this->videoSystem->draw(std::static_pointer_cast<IView>(this->player->getCurrentTexture()));
}

void PlayingState::executePostLoad() {
	this->soundSystem->setMusicLevel(8);
	this->soundSystem->playMusic("castle");
}

void PlayingState::makeEnemies() {
	float x = this->levelManager->getCurrentStage()->playerStartPosX;
	float y = this->levelManager->getCurrentStage()->playerStartPosY;

	auto textureManager = this->videoSystem->getTextureManager();
	auto orcTextures = textureManager->getTexturesFromSpriteSheet(
		Paths::CHARACTERS_DIR + "orcs.png",
		{
			10, // death
			10, // attack
			10, // walk
			10, // idle1
			10, // idle2
		}
	);

	auto idle1 = std::make_shared<AnimationView>(orcTextures[0], false, 0.15f, 120, 120, 3);
	auto idle2 = std::make_shared<AnimationView>(orcTextures[1], false, 0.15f, 120, 120, 3);
	auto walk = std::make_shared<AnimationView>(orcTextures[2], true, 0.15f, 120, 120, 3);
	auto attack = std::make_shared<AnimationView>(orcTextures[3], false, 0.15f, 120, 120, 3);
	auto death = std::make_shared<AnimationView>(orcTextures[4], false, 0.15f, 120, 120, 3);
	auto hitbox = std::make_shared<Hitbox>(x, y, 40, 40);

	std::vector<std::shared_ptr<AnimationView>> idleVec = { idle1, idle2 };
	std::vector<std::shared_ptr<AnimationView>> walkVec = { walk };
	std::vector<std::shared_ptr<AnimationView>> attackVec = { attack };

	auto movable = std::make_shared<Movable>(hitbox, 150.f);

	auto controllableParams = std::make_shared<ControllableParameters>(
		this->physicsSystem->getTimeModifier(),
		movable->maxSpeed,
		movable->doesCollide
	);

	auto enemy = std::make_shared<Entity>(
		movable,
		idleVec,
		attackVec,
		walkVec,
		death
	);

	this->enemyTest = std::make_unique<IController>(
		enemy,
		this->levelManager->getScoreGrid(),
		this->levelManager->getTileSize(),
		this->levelManager->getH(),
		this->levelManager->getW()
	);

	this->physicsSystem->addMovable(movable);
}