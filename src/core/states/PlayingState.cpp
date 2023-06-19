#include "PlayingState.h"

PlayingState::PlayingState(
	bool& stateChange,
	std::shared_ptr<InputSystem>& inputSystem,
	std::shared_ptr<VideoSystem>& videoSystem,
	std::shared_ptr<PhysicsSystem>& physicsSystem,
	std::shared_ptr<SoundSystem>& soundSystem,
	std::shared_ptr<GeneralSystem>& generalSystem
) : IState(stateChange, inputSystem, videoSystem, physicsSystem, soundSystem, generalSystem) {
	// Local utilities
	{
		this->updateBarrier = std::make_unique<Barrier>(4);
	}

	// Map/Level
	{
		int tileSize = 64;

		auto castleStage = std::make_shared<CastleStage>(
			std::static_pointer_cast<IGenerator>(std::make_shared<BSPGenerator>(50, 100, 5)),
			this->videoSystem->getTextureManager(),
			this->soundSystem,
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

	// Sound
	{
		this->soundSystem->loadMusic(Paths::MUSIC_DIR + "dark_forest.mp3", "castle");

		this->soundSystem->loadSound(Paths::SFX_DIR + "chest.mp3", "chest");
		this->soundSystem->loadSound(Paths::SFX_DIR + "iron_door.mp3", "iron_door");
		this->soundSystem->loadSound(Paths::SFX_DIR + "grunt_large_1.mp3", "grunt_large_1");
		this->soundSystem->loadSound(Paths::SFX_DIR + "grunt_large_2.mp3", "grunt_large_2");
		this->soundSystem->loadSound(Paths::SFX_DIR + "swoosh_1.mp3", "swoosh_1");
		this->soundSystem->loadSound(Paths::SFX_DIR + "swoosh_2.mp3", "swoosh_2");
	}

	//Player
	{
		float playerX = this->levelManager->getCurrentStage()->playerStartPosX;
		float playerY = this->levelManager->getCurrentStage()->playerStartPosY;
		float playerW = 40;
		float playerH = 40;

		auto textureManager = this->videoSystem->getTextureManager();

		auto rogueTextures = textureManager->getTexturesFromSpriteSheet(
			Paths::CHARACTERS_DIR + "rogue_.png",
			{
				10, // death
				9, // attack1
				9, // attack2
				9, // attack3
				10, // walk
				10, // idle1
				10, // idle2
			}
		);

		auto idle1 = std::make_shared<AnimationView>(rogueTextures[0], false, 0.15f, 120, 120, 3);
		auto idle2 = std::make_shared<AnimationView>(rogueTextures[1], false, 0.15f, 120, 120, 3);
		auto walk = std::make_shared<AnimationView>(rogueTextures[2], true, 0.15f, 120, 120, 3);
		auto attack1 = std::make_shared<AnimationView>(rogueTextures[3], false, 0.08f, 120, 120, 3);
		auto attack2 = std::make_shared<AnimationView>(rogueTextures[4], false, 0.08f, 120, 120, 3);
		auto attack3 = std::make_shared<AnimationView>(rogueTextures[5], false, 0.08f, 120, 120, 3);
		auto death = std::make_shared<AnimationView>(rogueTextures[6], false, 0.15f, 120, 120, 3);
		auto playerHitbox = std::make_shared<Hitbox>(playerX, playerY, playerW, playerH);

		std::vector<std::shared_ptr<AnimationView>> idleVec = { idle1, idle2 };
		std::vector<std::shared_ptr<AnimationView>> walkVec = { walk };
		std::vector<std::shared_ptr<AnimationView>> attackVec = { attack1, attack2, attack3 };

		float rangeVertical = 24;
		float rangeHorizontal = 16;
		auto combatableComponent = std::make_shared<Combatable>(rangeVertical, rangeHorizontal, 100.f, 35.f);

		auto movable = std::make_shared<Movable>(playerHitbox, combatableComponent, 150.f);

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

		combatableComponent->onAttack = [&soundSystem = soundSystem]() {
			rand() % 2 == 0 ? soundSystem->playSound("swoosh_1")
							: soundSystem->playSound("swoosh_2");
		};

		combatableComponent->onInteract = std::bind(&Player::interactWithEnemy, std::ref(player), std::placeholders::_1);
		combatableComponent->onDamageTaken = []() { };
		combatableComponent->onDeath = []() { };


		this->physicsSystem->addMovable(movable);

		this->videoSystem->setCameraSubject(playerHitbox);
		this->videoSystem->drawRelativeToCamera(true);
	}

	this->physicsSystem->addMovable(this->videoSystem->getCamera()->getFocusZone());

	this->videoSystem->endTransition();

	this->makeEnemies();

	{
		auto interactables = this->levelManager->getCurrentStage()->getInteractables();

		for (auto& i : interactables) {
			this->inputSystem->addEventCallback(i);
		}
	}

	// Setting static light sources
	{
		auto lights = this->levelManager->getCurrentStage()->getLightSourcePositions();

		auto shader = this->videoSystem->getShader("base");
		shader->use();
		
		auto loc = shader->getUniformLocation("uPointLightPositions");
		glUniform2fv(loc, lights.size(), glm::value_ptr(lights[0]));
		
		glUniform1i(shader->getUniformLocation("nLightSources"), lights.size());
	}
}

PlayingState::~PlayingState() {}

void PlayingState::handleInput() {
	auto input = this->inputSystem->getInput();
	
	this->player->handleInput(input);

	for (const auto& e : this->enemies) {
		e->feedInputToEntity();
	}
}

void PlayingState::update(float dTime) {
	this->generalSystem->queueThreadJob([
			dTime, 
			&player = player, 
			&updateBarrier = updateBarrier, 
			&levelManager = levelManager,
			&inputSystem = inputSystem]() {
		auto p = player->getMovableComponent();
		levelManager->updateScoreGrid(dTime, p->hitbox->x, p->hitbox->y);
		inputSystem->checkPlayerInteractables(p->hitbox->x, p->hitbox->y, p->hitbox->w, p->hitbox->h);

		updateBarrier->wait();
	});
	
	this->generalSystem->queueThreadJob([dTime, &physicsSystem = physicsSystem, &updateBarrier = updateBarrier, &enemies = enemies]() {
		physicsSystem->update(dTime);

		for (const auto& e : enemies) {
			e->update(dTime);
		}

		updateBarrier->wait();
	});

	this->generalSystem->queueThreadJob([
			dTime, 
			&player = player, 
			&levelManager = levelManager, 
			&updateBarrier = updateBarrier, 
			&enemies = enemies]() {
		player->update(dTime);

		levelManager->getCurrentStage()->update(dTime);

		auto tileMap = levelManager->getTileMap();
		for (int i = 0; i < tileMap.size(); i++) {
			for (int j = 0; j < tileMap[i].size(); j++) {
				tileMap[i][j]->clearAdditionalViews();
			}
		}

		auto p = player->getMovableComponent()->hitbox;
		int i = tileMap.size() - p->y / 64;
		int j = p->x / 64;

		tileMap[i][j]->addAdditionalView(std::static_pointer_cast<IView>(player->getCurrentTexture()));

		for (const auto& e : enemies) {
			auto p = e->getEntity()->getMovableComponent()->hitbox;
			int i = tileMap.size() - p->y / 64;
			int j = p->x / 64;

			tileMap[i][j]->addAdditionalView(std::static_pointer_cast<IView>(e->getEntity()->getCurrentTexture()));
		}

		updateBarrier->wait();
	});

	this->updateBarrier->wait();

	this->videoSystem->updateCamera(dTime);
}

void PlayingState::render() {
	auto tileMap = this->levelManager->getTileMap();
	const auto& ph = this->player->getMovableComponent()->hitbox;

	auto px = ph->x;
	auto py = ph->y;

	this->videoSystem->setFloatUniform("playerX", px);
	this->videoSystem->setFloatUniform("playerY", py);

	for (int i = 0; i < tileMap.size(); i++) {
		for (int j = tileMap[i].size() - 1; j >= 0 ; j--) {
			if (tileMap[i][j] != nullptr) {
				this->videoSystem->draw(std::static_pointer_cast<IView>(tileMap[i][j]->getView()));

				auto decoration = tileMap[i][j]->getDecoration();

				if (tileMap[i][j]->type == IGenerator::DOOR_VERTICAL) {
					auto adViews = tileMap[i][j]->getAdditionalViews();
					for (auto v : adViews) {
						this->videoSystem->draw(v);
					}

					this->videoSystem->draw(decoration);
				
				} else {
					if (decoration != nullptr) {
						this->videoSystem->draw(decoration);
					}

					auto adViews = tileMap[i][j]->getAdditionalViews();
					for (auto v : adViews) {
						this->videoSystem->draw(v);
					}
				}
			}
		}
	}

	/*for (const auto& e : this->enemies) {
		this->videoSystem->draw(std::static_pointer_cast<IView>(e->getEntity()->getCurrentTexture()));
	}

	this->videoSystem->draw(std::static_pointer_cast<IView>(this->player->getCurrentTexture()));*/
}

void PlayingState::executePostLoad() {
	this->soundSystem->setMusicLevel(8);
	this->soundSystem->playMusic("castle");
}

void PlayingState::makeEnemies() {
	auto textureManager = this->videoSystem->getTextureManager();
	auto orcTextures = textureManager->getTexturesFromSpriteSheet(
		Paths::CHARACTERS_DIR + "orcs.png",
		{
			// light
			10, // death
			10, // attack
			10, // walk
			10, // idle1
			10, // idle2
			// heavy
			10, // death
			10, // attack
			10, // walk
			10, // idle1
			10, // idle2
		}
	);

	for (auto p : this->levelManager->getCurrentStage()->getEnemyLightStart()) {
		float eps = rand() % 7 / 100.f;
		auto idle1 = std::make_shared<AnimationView>(orcTextures[5], false, 0.15f + eps, 120, 120, 3);
		auto idle2 = std::make_shared<AnimationView>(orcTextures[6], false, 0.15f + eps, 120, 120, 3);
		auto walk = std::make_shared<AnimationView>(orcTextures[7], true, 0.15f + eps, 120, 120, 3);
		auto attack = std::make_shared<AnimationView>(orcTextures[8], false, 0.11f + eps, 120, 120, 3);
		auto death = std::make_shared<AnimationView>(orcTextures[9], false, 0.15f + eps, 120, 120, 3);
		auto hitbox = std::make_shared<Hitbox>(p.first, p.second, 40, 40);

		std::vector<std::shared_ptr<AnimationView>> idleVec = { idle1, idle2 };
		std::vector<std::shared_ptr<AnimationView>> walkVec = { walk };
		std::vector<std::shared_ptr<AnimationView>> attackVec = { attack };

		float rangeVertical = 24;
		float rangeHorizontal = 24;
		auto combatableComponent = std::make_shared<Combatable>(rangeVertical, rangeHorizontal, 100.f, 15.f);

		auto movable = std::make_shared<Movable>(hitbox, combatableComponent, 150.f);

		this->physicsSystem->addMovable(movable);
		movable->maxSpeed = movable->maxSpeed - 10 - rand() % 50;

		auto enemy = std::make_shared<Entity>(
			movable,
			idleVec,
			attackVec,
			walkVec,
			death
		);

		auto e = std::make_unique<IController>(
			enemy,
			this->levelManager->getScoreGrid(),
			this->levelManager->getTileSize(),
			this->levelManager->getH(),
			this->levelManager->getW()
		);

		//combatableComponent->onInteract = std::bind(&Entity::interactWithEnemy, std::ref(e), std::placeholders::_1);
		combatableComponent->onInteract = [](std::shared_ptr<Movable>& m) {};
		combatableComponent->onAttack = []() {};

		combatableComponent->setDamageTakenCallback([&soundSystem = this->soundSystem]() {
			rand() % 2 == 0 ? soundSystem->playSound("grunt_large_1") 
							: soundSystem->playSound("grunt_large_2");
		});
		combatableComponent->onDeath = [&physicsSystem = this->physicsSystem, &movable = enemy->getMovableComponent()]() {
			physicsSystem->removeMovable(movable);
		};

		this->enemies.push_back(std::move(e));
	}

	for (auto p : this->levelManager->getCurrentStage()->getEnemyHeavyStart()) {
		float eps = rand() % 10 / 100.f;
		auto idle1 = std::make_shared<AnimationView>(orcTextures[0], false, 0.15f + eps, 120, 120, 3);
		auto idle2 = std::make_shared<AnimationView>(orcTextures[1], false, 0.15f + eps, 120, 120, 3);
		auto walk = std::make_shared<AnimationView>(orcTextures[2], true, 0.15f + eps, 120, 120, 3);
		auto attack = std::make_shared<AnimationView>(orcTextures[3], false, 0.12f + eps, 120, 120, 3);
		auto death = std::make_shared<AnimationView>(orcTextures[4], false, 0.15f + eps, 120, 120, 3);
		auto hitbox = std::make_shared<Hitbox>(p.first, p.second, 40, 40);

		std::vector<std::shared_ptr<AnimationView>> idleVec = { idle1, idle2 };
		std::vector<std::shared_ptr<AnimationView>> walkVec = { walk };
		std::vector<std::shared_ptr<AnimationView>> attackVec = { attack };

		float rangeVertical = 24;
		float rangeHorizontal = 24;
		auto combatableComponent = std::make_shared<Combatable>(rangeVertical, rangeHorizontal, 150.f, 15.f);

		auto movable = std::make_shared<Movable>(hitbox, combatableComponent, 150.f);
		movable->maxSpeed = movable->maxSpeed - rand() % 30;

		this->physicsSystem->addMovable(movable);

		auto enemy = std::make_shared<Entity>(
			movable,
			idleVec,
			attackVec,
			walkVec,
			death
		);

		auto e = std::make_unique<IController>(
			enemy,
			this->levelManager->getScoreGrid(),
			this->levelManager->getTileSize(),
			this->levelManager->getH(),
			this->levelManager->getW()
		);

		combatableComponent->onInteract = [](std::shared_ptr<Movable>& m) {};
		combatableComponent->onAttack = []() {};
		combatableComponent->onDamageTaken = [&soundSystem = this->soundSystem]() {
			rand() % 2 == 0 ? soundSystem->playSound("grunt_large_1")
							: soundSystem->playSound("grunt_large_2");
		};

		combatableComponent->onDeath = [&physicsSystem = this->physicsSystem, &movable = enemy->getMovableComponent()]() {
			physicsSystem->removeMovable(movable);
		};

		this->enemies.push_back(std::move(e));
	}


}