#include "CastleStage.h"

CastleStage::CastleStage(
	std::shared_ptr<IGenerator>& generator,
	std::shared_ptr<TextureManager>& textureManager,
	int tileSize
) : IStage(generator, textureManager, tileSize) {
	this->texturesDir = Paths::TILESETS_DIR +  "/castle/";
}

CastleStage::~CastleStage() { }

void CastleStage::generateStage() {
	auto stageTemplate = this->generateTemplate();
	auto stageTiles = this->findTileType(stageTemplate);
	
	this->loadTextures();
	this->createTileMap(stageTemplate, stageTiles);
}

char** CastleStage::findTileType(char** stageTemplate) {
	int w = generator->getW();
	int h = generator->getH();
	char** stageTiles = new char* [h];
	for (int i = 0; i < h; i++) {
		stageTiles[i] = new char[w];

		for (int j = 0; j < w; j++) {
			if (stageTemplate[i][j] == IGenerator::EMPTY) {
				if (
					stageTemplate[i][j - 1] == IGenerator::WALL &&
					stageTemplate[i][j + 1] == IGenerator::WALL &&
					stageTemplate[i - 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corridor_deadend_top;
				} else if (
					stageTemplate[i][j - 1] == IGenerator::WALL &&
					stageTemplate[i - 1][j] == IGenerator::WALL &&
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corridor_deadend_left;
				} else if (
					stageTemplate[i][j + 1] == IGenerator::WALL &&
					stageTemplate[i - 1][j] == IGenerator::WALL &&
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corridor_deadend_right;
				} else if (
					stageTemplate[i][j - 1] == IGenerator::WALL &&
					stageTemplate[i][j + 1] == IGenerator::WALL &&
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corridor_deadend_bottom;
				} else if (
					stageTemplate[i][j + 1] == IGenerator::WALL &&
					stageTemplate[i - 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corner_top_right;
				} else if (
					stageTemplate[i][j - 1] == IGenerator::WALL &&
					stageTemplate[i - 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corner_top_left;
				} else if (
					stageTemplate[i][j - 1] == IGenerator::WALL &&
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corner_bottom_left;
				} else if (
					stageTemplate[i][j + 1] == IGenerator::WALL &&
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corner_bottom_right;
				} else if (
					stageTemplate[i - 1][j] == IGenerator::WALL &&
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = corridor_horizontal;
				} else if (
					stageTemplate[i][j - 1] == IGenerator::WALL &&
					stageTemplate[i][j + 1] == IGenerator::WALL
				) {
					stageTiles[i][j] = corridor_vertical;
				} else if (
					stageTemplate[i - 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = top_wall;
				} else if (
					stageTemplate[i + 1][j] == IGenerator::WALL
				) {
					stageTiles[i][j] = bottom_wall;
				} else if (
					stageTemplate[i][j - 1] == IGenerator::WALL
				) {
					stageTiles[i][j] = left_wall;
				} else if (
					stageTemplate[i][j + 1] == IGenerator::WALL
				) {
					stageTiles[i][j] = right_wall;
				} else {
					stageTiles[i][j] = no_wall;
				}
			}
			else if (i < h - 1 && stageTemplate[i][j] == IGenerator::WALL && stageTemplate[i + 1][j] == IGenerator::EMPTY) {
				if (
					stageTemplate[i + 1][j - 1] == IGenerator::WALL 
					&& stageTemplate[i + 1][j + 1] == IGenerator::WALL
				) {
					stageTiles[i][j] = wall_corridor_deadend;
				} else if (stageTemplate[i + 1][j - 1] == IGenerator::WALL) {
					stageTiles[i][j] = wall_left;
				} else if (stageTemplate[i + 1][j + 1] == IGenerator::WALL) {
					stageTiles[i][j] = wall_right;
				} else {
					stageTiles[i][j] = wall_base;
				}
			}
		}
	}

	return stageTiles;
}

void CastleStage::loadTextures() {
	this->floorTextures = this->textureManager->getTexturesFromSpriteSheet(
		this->texturesDir + "floor.png", 
		{
			1, // corridor T down
			1, // corridor T up
			1, // corridor T left
			1, // corridor T right
			2, // corridor vertical
			2, // corridor horizontal
			1, // corridor dead-end bottom
			1, // corridor dead-end right
			1, // corridor dead-end top
			1, // corridor dead-end left
			8, // no wall
			3, // right wall
			3, // left wall
			3, // bottom wall
			3, // top wall
			2, // corner bottom-right
			2, // corner bottom-left
			2, // corner top-right
			2, // corner top-left
		},
		16,
		16
	);

	this->wallTextures = this->textureManager->getTexturesFromSpriteSheet(
		this->texturesDir + "walls.png",
		{
			2, // stairway
			1, // corridor dead-end
			4, // base wall
			1, // wall right
			1, // wall left
		},
		16,
		32
	);
}

void CastleStage::createTileMap(char** stageTemplate, char** stageTiles) {
	int h = this->generator->getH();
	int w = this->generator->getW();

	this->tileMap = std::vector<std::vector<std::shared_ptr<Tile>>>(
		h,
		std::vector<std::shared_ptr<Tile>>(w, nullptr)
	);

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			float tileX = j * this->tileSize;
			float tileY = (h - i - 1) * this->tileSize;

			if (stageTemplate[i][j] == IGenerator::EMPTY) {
				int tileType = stageTiles[i][j];
				auto texture = this->floorTextures[tileType][rand() % this->floorTextures[tileType].size()];
				auto view = std::make_shared<View>(texture, tileX, tileY, this->tileSize / 16);
				this->tileMap[i][j] = std::make_shared<Tile>(view);
			} else if (i < h - 1 && stageTemplate[i][j] == IGenerator::WALL && stageTemplate[i + 1][j] == IGenerator::EMPTY) {
				int tileType = stageTiles[i][j];
				auto texture = this->wallTextures[tileType][rand() % this->wallTextures[tileType].size()];
				auto view = std::make_shared<View>(texture, tileX, tileY, this->tileSize / 16);
				this->tileMap[i][j] = std::make_shared<Tile>(view);
			}
		}
	}

	auto texture = this->floorTextures[0][0];
	auto view = std::make_shared<View>(texture, 0, 0, 2);
	this->tileMap[0][0] = std::make_shared<Tile>(view);
}