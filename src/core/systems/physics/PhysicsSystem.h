#pragma once

#include <SDL_timer.h>

#include "../ISystem.h"

class PhysicsSystem : public ISystem {
private:
	uint64_t prevTime;
	uint64_t currTime;

	float dTime;
	float totalTime;

public:
	PhysicsSystem();

	void init() override;

	void computeFrameDeltaTime();
	float getFrameDeltaTime();
	
	unsigned int getTotalTime();
};