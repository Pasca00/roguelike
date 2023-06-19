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
		float updateInterval = 0.08f
	);

	virtual void update(float dtime);

	void feedInputToEntity();

	std::shared_ptr<Entity> getEntity();

private:
	void findNextMove();

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