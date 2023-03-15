#include "IInteractable.h"

IInteractable::IInteractable(std::string trigger, std::function<void(void)> callback, int repeat) {
	this->trigger = trigger;
	this->callback = callback;
	this->repeat = repeat;
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