#include "LevelManager.h"

#include <queue>
#include <utility>

int manhattanDistance(int i1, int j1, int i2, int j2) {
	return abs(i1 - i2) + abs(j1 - j2);
}

LevelManager::LevelManager(std::shared_ptr<CastleStage>& castleStage, int tileSize) 
	: castleStage(castleStage) {
	this->tileSize = tileSize;

	this->scoringInterval = 0.5f;
	this->timeSinceLastScoring = 0.f;

	this->scoreGrid = this->castleStage->getGenerator()->getScoreGrid();
	this->visitedAdresses = std::vector<int*>();
}

std::vector<std::vector<std::shared_ptr<Tile>>> LevelManager::getTileMap() {
	return this->castleStage->getTileMap();
}

std::shared_ptr<CastleStage>& LevelManager::getCurrentStage() {
	return this->castleStage;
}

void LevelManager::updateScoreGrid(float dTime, float playerX, float playerY) {
	this->timeSinceLastScoring += dTime;

	auto generator = this->castleStage->getGenerator();

	if (this->timeSinceLastScoring >= this->scoringInterval) {
		this->timeSinceLastScoring = 0.f;

		int iCurr = generator->getH() - 1 - playerY / this->tileSize;
		int jCurr = playerX / this->tileSize;

		// If player hasn't moved, it's not necessary to update the grid values
		if (iCurr == this->iPreviousPlayer && jCurr == this->jPreviousPlayer) {
			return;
		}
		
		this->iPreviousPlayer = iCurr;
		this->jPreviousPlayer = jCurr;

		std::queue<std::pair<int, int>> q;

		auto tileMap = this->castleStage->getTileMap();

		// Reset all previously set values to 0
		for (auto i : this->visitedAdresses) {
			*i = 0;
		}
		this->visitedAdresses.clear();

		q.push({ iCurr, jCurr });

		int i = 0;
		while (!q.empty()) {
			auto pos = q.front();
			q.pop();

			this->visitedAdresses.push_back(&this->scoreGrid[pos.first][pos.second]);

			if (tileMap[pos.first][pos.second]->type == IGenerator::WALL) {
				this->scoreGrid[pos.first][pos.second] = 0;
			} else {
				this->scoreGrid[pos.first][pos.second] = 100 - 10 * manhattanDistance(iCurr, jCurr, pos.first, pos.second);
			}

			// If we reached a score of 0, ignore all neighbours
			if (this->scoreGrid[pos.first][pos.second] == 0) {
				continue;
			}

			if (pos.first > 1) {
				if (this->scoreGrid[pos.first - 1][pos.second] == 0) {
					q.push({ pos.first - 1, pos.second });
				}
			}

			if (pos.first < generator->getH() - 2) {
				if (this->scoreGrid[pos.first + 1][pos.second] == 0) {
					q.push({ pos.first + 1, pos.second });
				}
			}

			if (pos.second > 1) {
				if (this->scoreGrid[pos.first][pos.second - 1] == 0) {
					q.push({ pos.first, pos.second - 1 });
				}
			}

			if (pos.first < generator->getW() - 2) {
				if (this->scoreGrid[pos.first][pos.second + 1] == 0) {
					q.push({ pos.first, pos.second + 1 });
				}
			}
		}
	}
}

int** LevelManager::getScoreGrid() {
	return this->scoreGrid;
}

int LevelManager::getTileSize() {
	return this->tileSize;
}

int LevelManager::getH() {
	return this->castleStage->getGenerator()->getH();
}

int LevelManager::getW() {
	return this->castleStage->getGenerator()->getW();
}