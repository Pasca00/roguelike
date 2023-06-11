#pragma once

#include <set>

#include "../../Entity.h"


class IController {
protected:
	int** scoreGrid;
	int tileSize;
	int mapH;
	int mapW;

	float timeSinceLastUpdate;
	float updateInterval;

	std::shared_ptr<Entity> controlledEntity;
	std::shared_ptr<Input> simulatedInput;

	std::set<std::pair<int, int>> openSet;
	std::set<std::pair<int, int>> closedSet;


public:
	IController(
		std::shared_ptr<Entity> controlledEntity, 
		int** scoreGrid, 
		int tileSize, 
		int mapH,
		int mapW,
		float updateInterval = 0.15f
	) {
		this->scoreGrid = scoreGrid;
		this->tileSize = tileSize;
		this->mapH = mapH;
		this->mapW = mapW;

		this->updateInterval = updateInterval;
		this->controlledEntity = controlledEntity;

		this->timeSinceLastUpdate = 0;

		this->simulatedInput = std::make_shared<Input>();
	}

	virtual void update(float dtime) {
		this->timeSinceLastUpdate += dtime;
		this->controlledEntity->update(dtime);

		if (this->timeSinceLastUpdate >= this->updateInterval) {
			this->timeSinceLastUpdate = 0;

			this->findNextMove();
		}
	}

	void feedInputToEntity() {
		this->controlledEntity->handleInput(this->simulatedInput);
	}

	std::shared_ptr<Entity> getEntity() {
		return this->controlledEntity;
	}

private:
	void findNextMove() {
		this->simulatedInput->clear();

		auto h = this->controlledEntity->getMovableComponent()->hitbox;
		
		int iCurr = this->mapH - 1 - h->y / tileSize;
		int jCurr = h->x / tileSize;

		//printf("score: %d\n", this->scoreGrid[iCurr][jCurr]);

		int maxScore = scoreGrid[iCurr][jCurr];
		int nextPos = NONE;

		/* -------------------------- */
		if (scoreGrid[iCurr - 1][jCurr - 1] > maxScore) {
			maxScore = scoreGrid[iCurr - 1][jCurr - 1];
			nextPos = UPPER_LEFT;
		}

		if (scoreGrid[iCurr - 1][jCurr] > maxScore) {
			maxScore = scoreGrid[iCurr - 1][jCurr];
			nextPos = UP;
		}

		if (scoreGrid[iCurr - 1][jCurr + 1] > maxScore) {
			maxScore = scoreGrid[iCurr - 1][jCurr + 1];
			nextPos = UPPER_RIGHT;
		}

		/* -------------------------- */
		if (scoreGrid[iCurr][jCurr - 1] > maxScore) {
			maxScore = scoreGrid[iCurr][jCurr - 1];
			nextPos = LEFT;
		}

		if (scoreGrid[iCurr][jCurr + 1] > maxScore) {
			maxScore = scoreGrid[iCurr][jCurr + 1];
			nextPos = RIGHT;
		}

		/* -------------------------- */
		if (scoreGrid[iCurr + 1][jCurr - 1] > maxScore) {
			maxScore = scoreGrid[iCurr + 1][jCurr - 1];
			nextPos = LOWER_LEFT;
		}

		if (scoreGrid[iCurr + 1][jCurr] > maxScore) {
			maxScore = scoreGrid[iCurr + 1][jCurr];
			nextPos = DOWN;
		}

		if (scoreGrid[iCurr + 1][jCurr + 1] > maxScore) {
			maxScore = scoreGrid[iCurr + 1][jCurr + 1];
			nextPos = LOWER_RIGHT;
		}

		switch (nextPos) {
		case UPPER_LEFT:
			this->simulatedInput->setActionValue("UP", true);
			this->simulatedInput->setActionValue("LEFT", true);
			break;

		case UP:
			this->simulatedInput->setActionValue("UP", true);
			break;

		case UPPER_RIGHT:
			this->simulatedInput->setActionValue("UP", true);
			this->simulatedInput->setActionValue("RIGHT", true);
			break;

		case LOWER_LEFT:
			this->simulatedInput->setActionValue("DOWN", true);
			this->simulatedInput->setActionValue("LEFT", true);
			break;

		case DOWN:
			this->simulatedInput->setActionValue("DOWN", true);
			break;

		case LOWER_RIGHT:
			this->simulatedInput->setActionValue("DOWN", true);
			this->simulatedInput->setActionValue("RIGHT", true);
			break;

		case LEFT:
			this->simulatedInput->setActionValue("LEFT", true);
			break;

		case RIGHT:
			this->simulatedInput->setActionValue("RIGHT", true);
		}
	}

	/*void findPathToTarget(int targetX, int targetY) {
		auto m = this->controlledEntity->getMovableComponent();

		int startX;
		int startY;

		bool closedList[]
		openSet.insert({ startY, startX });

		while (!openSet.empty()) {
			auto curr = *openSet.begin();

			openSet.erase(openSet.begin());
		}
	}*/

	// 1 2 3
	// 4 0 5
	// 6 7 8
	static const int UPPER_LEFT  = 1;
	static const int UP			 = 2;
	static const int UPPER_RIGHT = 3;
	static const int RIGHT		 = 4;
	static const int NONE		 = 0;
	static const int LEFT		 = 5;
	static const int LOWER_LEFT  = 6;
	static const int DOWN		 = 7;
	static const int LOWER_RIGHT = 8;

};