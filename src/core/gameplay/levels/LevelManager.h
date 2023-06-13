#pragma once

#include <memory>
#include <vector>

#include "stages/IStage.h"
#include "stages/CastleStage.h"

class LevelManager {
private:
	int tileSize;

	std::shared_ptr<CastleStage> castleStage;

	float timeSinceLastScoring;
	float scoringInterval;

	int iPreviousPlayer;
	int jPreviousPlayer;

	int** scoreGrid;

	std::vector<int*> visitedAdresses;

	int noiseDampening;

public:
	LevelManager(std::shared_ptr<CastleStage>& castleStage, int tileSize);

	std::vector<std::vector<std::shared_ptr<Tile>>> getTileMap();

	std::shared_ptr<CastleStage>& getCurrentStage();

	void updateScoreGrid(float dTime, float playerX, float playerY);
	int** getScoreGrid();

	int getTileSize();
	int getH();
	int getW();

	int* getNoiseDampeningPointer();
};