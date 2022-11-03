#include "InputSystem.h"

#include <iostream>

const std::string InputSystem::keyMappingsFilename = "keys";

InputSystem::InputSystem() { }

void InputSystem::init() {
	this->createDefaultKeyMappings();
}

bool InputSystem::settingsFileExists() {
	std::ifstream file(Paths::SETTINGS_DIR + keyMappingsFilename);
	return file.good();
}

void InputSystem::createDefaultKeyMappings() {
	auto keyMappingsPath = Paths::SETTINGS_DIR + keyMappingsFilename;
	std::ifstream in(keyMappingsPath.c_str());
}

void InputSystem::readKeyMappings() {
	// TODO
	/*
	if (!this->settingsFileExists()) {
		this->createDefaultKeyMappings();
	}
	*/

	this->keyMappings = {
		{"UP",    SDLK_w},
		{"DOWN",  SDLK_s},
		{"LEFT",  SDLK_a},
		{"RIGHT", SDLK_d},

		{"ENTER", SDLK_RETURN},

		{"PAUSE", SDLK_ESCAPE},
	};
}