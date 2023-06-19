#include "StatusEffect.h"

#include "../inventory/GlobalEffectManager.h"
#include "../../systems/physics/Movable.h"

//#include "../../.h"

StatusEffect::StatusEffect(
	float totalDuration,
	float applyInterval, 
	std::function<void(std::shared_ptr<Movable>&)> callback
) {
	this->totalDuration = totalDuration;
	this->applyInterval = applyInterval;
	this->callback = callback;

	this->timeSinceLastTick = 0.0;

	this->done = false;
}

void StatusEffect::apply(float dtime, std::shared_ptr<Movable>& target) {
	if (this->done) {
		return;
	}

	this->timeSinceLastTick += dtime;

	if (this->timeSinceLastTick >= this->applyInterval) {
		this->callback(target);

		this->timeSinceLastTick = 0;

		this->totalDuration -= applyInterval;
	}

	if (this->totalDuration <= 0) {
		this->done = true;
	}
}

void StatusEffect::setCallback(std::function<void(std::shared_ptr<Movable>&)> callback) {
	this->callback = callback;
}

bool StatusEffect::isDone() {
	return this->done;
}