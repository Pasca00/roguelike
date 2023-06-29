#pragma once

#include <functional>

class Player;

class UsableItem {
public:
	UsableItem();

	bool inUse;

	float energyCost;

	std::function<void(Player*)> onEffectStart;
	std::function<void(Player*)> onEffectEnd;
};