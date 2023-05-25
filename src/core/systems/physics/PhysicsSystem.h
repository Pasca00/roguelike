#pragma once

#include <SDL_timer.h>

#include <vector>
#include <memory>

#include "../ISystem.h"
#include "Movable.h"

class PhysicsSystem : public ISystem {
private:
	uint64_t prevTime;
	uint64_t currTime;

	float dTime;
	float totalTime;

	std::vector<std::shared_ptr<Movable>> movables;

	float friction;

public:
	PhysicsSystem();

	void init() override;

	void update(float dtime);

	void computeFrameDeltaTime();
	float getFrameDeltaTime();
	
	unsigned int getTotalTime();

	void addMovable(std::shared_ptr<Movable>& movable);
};