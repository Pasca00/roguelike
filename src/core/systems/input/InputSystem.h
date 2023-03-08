#pragma once

#include <fstream>
#include <string>
#include <unordered_map>

#include <SDL.h>

#include "../ISystem.h"
#include "../../Paths.h"

struct Input {
	std::unordered_map<std::string, bool> actions;

	Input();

	void setActionValue(const std::string& action, bool value);
	bool getAction(const std::string& action);
};

class InputSystem : public ISystem {
private:
	static const std::string keyMappingsFilename;

	std::unordered_map<SDL_Keycode, std::string> keyActionMappings;
	std::shared_ptr<Input> input;

	bool settingsFileExists();
	void createDefaultKeyMappings();
	void readKeyMappings();

public:
	InputSystem();

	void init();

	void collectInput();

	std::shared_ptr<Input> getInput();
};