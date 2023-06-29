#include "GuardianController.h"

GuardianController::GuardianController(
	std::shared_ptr<Entity> controlledEntity,
	int** scoreGrid,
	int tileSize,
	int mapH,
	int mapW,
	float updateInterval
) : IController(controlledEntity, scoreGrid, tileSize, mapH, mapW, updateInterval) { }

void GuardianController::update(float dtime) {
	this->timeSinceLastUpdate += dtime;
	this->controlledEntity->update(dtime);

	if (this->timeSinceLastUpdate >= this->updateInterval) {
		this->timeSinceLastUpdate = 0;

		this->simulatedInput->clear();
		this->wakeUp();
		this->findNextMove();
	}
}

void GuardianController::wakeUp() {
	if (this->controlledEntity->getCurrentState() != EntityState::DORMANT) {
		return;
	}

	auto h = this->controlledEntity->getMovableComponent()->hitbox;

	int iCurr = this->mapH - h->y / tileSize;
	int jCurr = h->x / tileSize;

	if (scoreGrid[iCurr][jCurr] >= 95.f) {
		this->simulatedInput->setActionValue("WAKE_UP", true);
	}
}