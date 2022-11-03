#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

#include <SDL_keycode.h>

#include "../ISystem.h"
#include "../../Paths.h"

class InputSystem : public ISystem {
private:
	static const std::string keyMappingsFilename;

	std::unordered_map<std::string, SDL_KeyCode> keyMappings;

	bool settingsFileExists();
	void createDefaultKeyMappings();
	void readKeyMappings();

public:
	InputSystem();

	void init();
};