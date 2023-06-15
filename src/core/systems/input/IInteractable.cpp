#include "IInteractable.h"

IInteractable::IInteractable(
	std::string trigger, 
	std::function<void(void)> callback, 
	float x,
	float y,
	float w,
	float h,
	int repeat,
	bool enabled
) {
	this->trigger = trigger;
	this->callback = callback;
	this->repeat = repeat;
	this->enabled = enabled;
	this->rect = std::make_shared<Hitbox>(x, y, w, h);
}

void IInteractable::call() {
	if (this->repeat == -1) {
		this->callback();

		return;
	}

	if (this->repeat > 0) {
		this->callback();
		this->repeat--;
	}
}

bool IInteractable::isDone() {
	return this->repeat == 0;
}

void IInteractable::enable() {
	this->enabled = true;
}

void IInteractable::disable() {
	this->enabled = false;
}

bool IInteractable::isEnabled() {
	return this->enabled;
}

std::string& IInteractable::getTrigger() {
	return this->trigger;
}

std::shared_ptr<Hitbox>& IInteractable::getHitbox() {
	return this->rect;
}