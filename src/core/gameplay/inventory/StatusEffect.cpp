#include "StatusEffect.h"

StatusEffect::StatusEffect(
	float totalDuration,
	float applyInterval, 
	std::function<void(std::shared_ptr<Entity>&)> callback
) {
	this->totalDuration = totalDuration;
	this->applyInterval = applyInterval;
	this->callback = callback;

	this->timeSinceLastTick = 0.0;

	this->totalDuration = false;
}

void StatusEffect::apply(float dtime, std::shared_ptr<Entity>& target) {
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

void StatusEffect::setCallback(std::function<void(std::shared_ptr<Entity>&)> callback) {
	this->callback = callback;
}

bool StatusEffect::isDone() {
	return this->done;
}