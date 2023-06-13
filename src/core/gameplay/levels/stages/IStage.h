#pragma once

#include "../../../systems/video/textures/TextureManager.h"
#include "../../../systems/video/views/AnimationView.h"
#include "../generators/IGenerator.h"
#include "../generators/BSPGenerator.h"
#include "../tiles/Tile.h"

class IStage {
protected:
	std::shared_ptr<IGenerator> generator;
	std::shared_ptr<TextureManager> textureManager;

	std::vector<std::vector<std::shared_ptr<Tile>>> tileMap;

	std::vector<std::vector<std::shared_ptr<Texture>>> floorTextures;
	std::vector<std::vector<std::shared_ptr<Texture>>> wallTextures;
	std::vector<std::vector<std::shared_ptr<Texture>>> ceilingTextures;

	std::vector<std::shared_ptr<AnimationView>> torches;
	std::vector<std::shared_ptr<Texture>> torchTextures;

	std::vector<std::shared_ptr<Texture>> wallDecorations;
	std::vector<std::shared_ptr<Texture>> floorDecorations;

	std::vector<std::shared_ptr<Texture>> chestTextures;

	int tileSize;

	virtual void loadTextures() = 0;
	virtual void createTileMap(char** stageTemplate, char** stageTiles) = 0;
	virtual void placeItems() = 0;

public:
	float playerStartPosX;
	float playerStartPosY;

	IStage(
		std::shared_ptr<IGenerator>& generator,
		std::shared_ptr<TextureManager>& textureManager,
		int tileSize = 32
	) : generator(generator), 
		textureManager(textureManager), 
		tileSize(tileSize), 
		playerStartPosX(0), 
		playerStartPosY(0) { }

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
};