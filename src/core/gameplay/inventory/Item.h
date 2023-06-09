#pragma once

#include <functional>
#include <string>

class Item {
private:
	std::string name;
	std::string description;

	uint8_t numberOfUses;
	std::function<void()> onUseCallback;

public:
	Item(std::string name, std::string description);
};