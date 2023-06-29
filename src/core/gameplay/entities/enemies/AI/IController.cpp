#include "IController.h"

IController::IController(
	std::shared_ptr<Entity> controlledEntity,
	int** scoreGrid,
	int tileSize,
	int mapH,
	int mapW,
	float updateInterval
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

void IController::update(float dtime) {
	this->timeSinceLastUpdate += dtime;
	this->controlledEntity->update(dtime);

	if (this->timeSinceLastUpdate >= this->updateInterval) {
		this->timeSinceLastUpdate = 0;

		this->simulatedInput->clear();
		this->findNextMove();
	}
}

void IController::feedInputToEntity() {
	this->controlledEntity->handleInput(this->simulatedInput);
}

std::shared_ptr<Entity> IController::getEntity() {
	return this->controlledEntity;
}

void IController::findNextMove() {
	if (!controlledEntity->isEnabled()) {
		return;
	}

	auto feared = this->controlledEntity->getMovableComponent()->combatableComponent->feared;

	auto h = this->controlledEntity->getMovableComponent()->hitbox;

	int iCurr = this->mapH - h->y / tileSize;
	int jCurr = h->x / tileSize;

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

	if (maxScore >= 90 && !feared) {
		this->simulatedInput->setActionValue("CLICK", true);
	}

	nextPos *= feared ? -1 : 1;

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