#include "InputSystem.h"

#include <iostream>

Input::Input() {
	this->actions = {
		{"UP",		 false},
		{"DOWN",	 false},
		{"LEFT",	 false},
		{"RIGHT",	 false},
		{"INTERACT", false},

		{"ENTER",	 false},

		{"PAUSE",	 false},
		{"QUIT",	 false},
	};
}

void Input::setActionValue(const std::string& action, bool value) {
	this->actions[action] = value;
}

bool Input::getAction(const std::string& action) {
	return this->actions[action];
}

const std::string InputSystem::keyMappingsFilename = "keys";

InputSystem::InputSystem() { }

void InputSystem::init() {
	this->input = std::make_shared<Input>();
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

	this->keyActionMappings = {
		{SDLK_w, "UP"},
		{SDLK_s, "DOWN"},
		{SDLK_a, "LEFT"},
		{SDLK_d, "RIGHT"},

		{SDLK_RETURN, "ENTER"},

		{SDLK_ESCAPE, "PAUSE"},
	};
}

void InputSystem::collectInput() {
	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_KEYDOWN: {
				auto action = this->keyActionMappings[e.key.keysym.sym];
				this->input->setActionValue(action, true);

				break;
			}

			case SDL_KEYUP: {
				auto action = this->keyActionMappings[e.key.keysym.sym];
				this->input->setActionValue(action, false);

				break;
			}

			case SDL_QUIT: {
				this->input->setActionValue("QUIT", true);
			}
		}
	}
}