#pragma once

#include "IController.h"

class GuardianController : public IController {
public:
	GuardianController(
		std::shared_ptr<Entity> controlledEntity,
		int** scoreGrid,
		int tileSize,
		int mapH,
		int mapW,
		float updateInterval = 0.08f
	);

	void wakeUp();

	void update(float dtime) override;
};