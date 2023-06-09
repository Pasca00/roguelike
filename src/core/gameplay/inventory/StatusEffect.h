#pragma once

#include <functional>

#include "../entities/Entity.h"

enum class EffectName {
	BURN,
	SLOW,
	FREEZE,
};

class StatusEffect {
private:
	std::function<void(std::shared_ptr<Entity>&)> callback;

	float totalDuration;
	float applyInterval;

	float timeSinceLastTick;

	bool done;

public:
	StatusEffect(
		float totalDuration = 5000.f,
		float applyInterval = 500.f,
		std::function<void(std::shared_ptr<Entity>&)> callback = nullptr
	);

	void apply(float dtime, std::shared_ptr<Entity>& target);

	void setCallback(std::function<void(std::shared_ptr<Entity>&)> callback);

	bool isDone();
};