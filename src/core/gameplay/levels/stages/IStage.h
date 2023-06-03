#pragma once

#include "../../../systems/video/textures/TextureManager.h"
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

	int tileSize;

	virtual void loadTextures() = 0;
	virtual void createTileMap(char** stageTemplate, char** stageTiles) = 0;

public:
	IStage(
		std::shared_ptr<IGenerator>& generator,
		std::shared_ptr<TextureManager>& textureManager,
		int tileSize = 32
	) : generator(generator), textureManager(textureManager), tileSize(tileSize) { }

	char** generateTemplate() {
		return this->generator->generateLayout();
	};

	virtual void generateStage() = 0;

	std::vector<std::vector<std::shared_ptr<Tile>>> getTileMap() {
		return this->tileMap;
	}

	TreeNode* getMapTree() {
		return this->generator->mapTree;
	}
};