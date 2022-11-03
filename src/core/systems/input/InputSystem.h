#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

#include <SDL.h>

#include "../ISystem.h"
#include "../../Paths.h"

class InputSystem : public ISystem {
private:
	static const std::string keyMappingsFilename;

	std::unordered_map<SDL_Keycode, std::string> keyMappings;
	std::unordered_map<std::string, bool> input;

	bool settingsFileExists();
	void createDefaultKeyMappings();
	void readKeyMappings();

public:
	InputSystem();

	void init();

	void collectInput();
};