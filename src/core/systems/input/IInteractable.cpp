#include "IInteractable.h"

IInteractable::IInteractable(
	std::string trigger, 
	std::function<void(void)> callback, 
	float x,
	float y,
	float w,
	float h,
	int repeat
) {
	this->trigger = trigger;
	this->callback = callback;
	this->repeat = repeat;
	this->rect = std::make_shared<Hitbox>(x, y, w, h);
}

void IInteractable::call() {
	if (this->repeat > 0) {
		this->callback();
		this->repeat--;
	}
}

bool IInteractable::isDone() {
	return this->repeat == 0;
}

std::string& IInteractable::getTrigger() {
	return this->trigger;
}