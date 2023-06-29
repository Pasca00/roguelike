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

protected:
	void findNextMove();

	// -3 -2 -1
	// -4  0  4
	//  1  2  3
	static const int UPPER_LEFT  = -3;
	static const int UP			 = -2;
	static const int UPPER_RIGHT = -1;
	static const int RIGHT		 = -4;
	static const int NONE		 = 0;
	static const int LEFT		 = 4;
	static const int LOWER_LEFT  = 1;
	static const int DOWN		 = 2;
	static const int LOWER_RIGHT = 3;

};