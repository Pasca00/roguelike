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

	this->noiseDampening = 10;
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

		std::queue<std::pair<std::pair<int, int>, int>> q;

		auto tileMap = this->castleStage->getTileMap();

		// Reset all previously set values to 0
		for (auto i : this->visitedAdresses) {
			*i = 0;
		}
		this->visitedAdresses.clear();

		q.push({ { iCurr, jCurr }, 100 });

		int i = 0;
		while (!q.empty()) {
			auto n = q.front();
			auto pos = n.first;
			auto score = n.second;
			q.pop();

			this->visitedAdresses.push_back(&this->scoreGrid[pos.first][pos.second]);

			if (tileMap[pos.first][pos.second]->type == IGenerator::WALL) {
				this->scoreGrid[pos.first][pos.second] = 0;
			} else {
				this->scoreGrid[pos.first][pos.second] = score;
			}

			// If we reached a score of 0, ignore all neighbours
			if (this->scoreGrid[pos.first][pos.second] == 0) {
				continue;
			}

			if (pos.first > 1) {
				if (this->scoreGrid[pos.first - 1][pos.second] == 0) {
					q.push({ { pos.first - 1, pos.second }, score - this->noiseDampening });
				}
			}

			if (pos.first < generator->getH() - 2) {
				if (this->scoreGrid[pos.first + 1][pos.second] == 0) {
					q.push({ { pos.first + 1, pos.second }, score - this->noiseDampening });
				}
			}

			if (pos.second > 1) {
				if (this->scoreGrid[pos.first][pos.second - 1] == 0) {
					q.push({ { pos.first, pos.second - 1 }, score - this->noiseDampening });
				}
			}

			if (pos.first < generator->getW() - 2) {
				if (this->scoreGrid[pos.first][pos.second + 1] == 0) {
					q.push({ { pos.first, pos.second + 1 }, score - this->noiseDampening });
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

int* LevelManager::getNoiseDampeningPointer() {
	return &this->noiseDampening;
}