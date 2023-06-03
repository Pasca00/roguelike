#pragma once

#include <memory>
#include <vector>

#include "stages/IStage.h"
#include "stages/CastleStage.h"

class LevelManager {
private:
	int tileSize;

	std::shared_ptr<CastleStage> castleStage;

public:
	LevelManager(std::shared_ptr<CastleStage>& castleStage, int tileSize);


	std::vector<std::vector<std::shared_ptr<Tile>>> getTileMap();

	std::shared_ptr<CastleStage>& getCurrentStage();
};