#pragma once

#include <string>
#include <functional>
#include <memory>

#include "../../gameplay/Hitbox.h"

class IInteractable {
private:
	std::function<void(void)> callback;
	std::string trigger;
	
	int repeat;

	std::shared_ptr<Hitbox> rect;

	bool enabled;

public:
	IInteractable(
		std::string trigger,
		std::function<void(void)> callback,
		float x = 0,
		float y = 0,
		float w = 0,
		float h = 0,
		int repeat = 1,
		bool enabled = true
	);
	
	void call();
	bool isDone();

	void enable();
	void disable();
	bool isEnabled();

	std::string& getTrigger();

	std::shared_ptr<Hitbox>& getHitbox();
};