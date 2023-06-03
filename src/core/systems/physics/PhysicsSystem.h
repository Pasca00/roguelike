#pragma once

#include <SDL_timer.h>

#include <vector>
#include <memory>

#include "../ISystem.h"
#include "Movable.h"
#include "../../gameplay/levels/tiles/Tile.h"
#include "../../gameplay/levels/generators/IGenerator.h"

class PhysicsSystem : public ISystem {
private:
	uint64_t prevTime;
	uint64_t currTime;

	float dTime;
	float totalTime;

	std::vector<std::shared_ptr<Movable>> movables;

	float friction;

	std::vector<std::vector<std::shared_ptr<Tile>>> currentTilemap;
	TreeNode* currentMapTree;

public:
	PhysicsSystem();

	void init() override;

	void update(float dtime);

	void computeFrameDeltaTime();
	float getFrameDeltaTime();
	
	unsigned int getTotalTime();

	void addMovable(std::shared_ptr<Movable>& movable);

	void setMap(std::vector<std::vector<std::shared_ptr<Tile>>>& tilemap, TreeNode* mapTree);
};