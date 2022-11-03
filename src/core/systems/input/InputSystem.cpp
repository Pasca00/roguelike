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

void InputSystem::readKeyMappings() {
	auto keyMappingsPath = Paths::SETTINGS_DIR + keyMappingsFilename;
	std::ifstream in(keyMappingsPath.c_str());
}

void InputSystem::createDefaultKeyMappings() {
	// TODO
	/*
	if (!this->settingsFileExists()) {
		this->createDefaultKeyMappings();
	}
	*/

	this->keyMappings = {
		{SDLK_w, "UP"},
		{SDLK_s, "DOWN"},
		{SDLK_a, "LEFT"},
		{SDLK_d, "RIGHT"},

		{SDLK_RETURN, "ENTER"},

		{SDLK_ESCAPE, "PAUSE"},
	};

	this->input = {
		{"UP",    false},
		{"DOWN",  false},
		{"LEFT",  false},
		{"RIGHT", false},

		{"ENTER", false},

		{"PAUSE", false},
	};
}

void InputSystem::collectInput() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_KEYDOWN: {
				auto action = this->keyMappings[e.key.keysym.sym];
				this->input[action] = true;

				break;
			}

			case SDL_KEYUP: {
				auto action = this->keyMappings[e.key.keysym.sym];
				this->input[action] = false;

				break;
			}
		}
	}
}