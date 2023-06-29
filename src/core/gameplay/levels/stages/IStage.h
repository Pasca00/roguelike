#pragma once

#include "../../../../glm/glm.hpp"

#include "../../../systems/sound/SoundSystem.h"
#include "../../../systems/input/IInteractable.h"
#include "../../../systems/video/textures/TextureManager.h"
#include "../../../systems/video/views/AnimationView.h"
#include "../../../gameplay/inventory/Item.h"
#include "../generators/IGenerator.h"
#include "../generators/BSPGenerator.h"
#include "../tiles/Tile.h"

enum class ItemIds {
	RUNE_OF_FIRE,
	RUNE_OF_ICE,
	TIME_RIFT,
	SERRATED_BLADE,
	DEADLY_POISON,
	PARANOIA,

	QUICK_HANDS,
	CREATINE_POWDER,
	EAGLE_EYES,

	// Usables
	GHOSTWALK,
	TIMEWALK,
};

class IStage {
protected:
	std::vector<ItemIds> itemPool = {
		/*ItemIds::RUNE_OF_FIRE,
		ItemIds::RUNE_OF_ICE,
		ItemIds::QUICK_HANDS,
		ItemIds::EAGLE_EYES,
		ItemIds::TIME_RIFT,*/
		//ItemIds::SERRATED_BLADE,
		//ItemIds::PARANOIA,
		//ItemIds::CREATINE_POWDER,
		ItemIds::TIMEWALK,
	};

	std::vector<ItemIds> usableItemPool = {
		ItemIds::GHOSTWALK,
		ItemIds::TIMEWALK,
	};

	std::shared_ptr<SoundSystem> soundSystem;

	std::shared_ptr<IGenerator> generator;
	std::shared_ptr<TextureManager> textureManager;

	std::vector<std::vector<std::shared_ptr<Tile>>> tileMap;

	std::vector<std::vector<std::shared_ptr<Texture>>> floorTextures;
	std::vector<std::vector<std::shared_ptr<Texture>>> wallTextures;
	std::vector<std::vector<std::shared_ptr<Texture>>> ceilingTextures;

	std::vector<std::shared_ptr<AnimationView>> animations;
	std::vector<std::shared_ptr<Texture>> torchTextures;
	std::vector<std::shared_ptr<Texture>> brasierTextures;

	std::vector<std::shared_ptr<Texture>> wallDecorations;
	std::vector<std::shared_ptr<Texture>> floorDecorations;

	std::vector<std::shared_ptr<Texture>> chestTextures;
	std::vector<std::shared_ptr<Texture>> crateTextures;
	std::vector<std::shared_ptr<Texture>> barrelTextures;

	std::vector<std::shared_ptr<Texture>> doorTextures;

	std::vector<std::shared_ptr<Texture>> scrollsTextures;
	std::vector<std::shared_ptr<Texture>> potionTextures;

	std::vector<std::unique_ptr<IInteractable>> interactables;

	std::vector<glm::vec2> lightPositions;

	std::vector<std::shared_ptr<Item>> items;

	int tileSize;

	float interactRange;

	std::vector<std::pair<float, float>> enemyHeavyPositions;
	std::vector<std::pair<float, float>> enemyLightPositions;

	std::vector<std::pair<float, float>> enemyGuardianPositions;
	std::pair<float, float> exitPosition;

	virtual void loadTextures() = 0;
	virtual void createTileMap(char** stageTemplate, char** stageTiles) = 0;
	virtual void placeItems() = 0;
	virtual void placePotions() = 0;
	virtual void placeDoors() = 0;
	virtual void placeExit() = 0;
	virtual void placeEnemiesStartPositions() = 0;

	virtual std::shared_ptr<Item> makeRandomItem(int i, int j) = 0;

public:
	float playerStartPosX;
	float playerStartPosY;

	IStage(
		std::shared_ptr<IGenerator>& generator,
		std::shared_ptr<TextureManager>& textureManager,
		std::shared_ptr<SoundSystem>& soundSystem,
		int tileSize = 32
	) : generator(generator),
		textureManager(textureManager),
		tileSize(tileSize),
		soundSystem(soundSystem),
		playerStartPosX(0), 
		playerStartPosY(0),
		interactRange(48.f) { }

	char** generateTemplate() {
		return this->generator->generateLayout();
	};

	virtual void generateStage() = 0;

	std::vector<std::vector<std::shared_ptr<Tile>>>& getTileMap() {
		return this->tileMap;
	}

	TreeNode* getMapTree() {
		return this->generator->mapTree;
	}

	std::shared_ptr<IGenerator>& getGenerator() {
		return this->generator;
	}

	std::vector<std::unique_ptr<IInteractable>> getInteractables() {
		return std::move(this->interactables);
	}

	std::vector<std::pair<float, float>>& getEnemyHeavyStart() {
		return this->enemyHeavyPositions;
	}

	std::vector<std::pair<float, float>>& getEnemyLightStart() {
		return this->enemyLightPositions;
	}

	std::vector<std::pair<float, float>>& getEnemyGuardiantStart() {
		return this->enemyGuardianPositions;
	}

	std::pair<float, float>& getExitPosition() {
		return this->exitPosition;
	}

	std::vector<glm::vec2>& getLightSourcePositions() {
		return this->lightPositions;
	}

	std::vector<std::shared_ptr<Item>>& getItems() {
		return this->items;
	}
};