#pragma once

#include <functional>

//#include "../../systems/physics/Movable.h"

class Movable;

class StatusEffect {
private:
	std::function<void(std::shared_ptr<Movable>&)> callback;

	float totalDuration;
	float applyInterval;

	float timeSinceLastTick;

	bool done;

public:
	StatusEffect(
		float totalDuration = 5000.f,
		float applyInterval = 500.f,
		std::function<void(std::shared_ptr<Movable>&)> callback = nullptr
	);

	void apply(float dtime, std::shared_ptr<Movable>& target);

	void setCallback(std::function<void(std::shared_ptr<Movable>&)> callback);

	bool isDone();
};