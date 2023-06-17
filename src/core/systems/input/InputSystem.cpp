#include "InputSystem.h"

#include <iostream>

Input::Input() {
	this->actions = {
		{"UP",		 false},
		{"DOWN",	 false},
		{"LEFT",	 false},
		{"RIGHT",	 false},
		{"INTERACT", false},
		{"CLICK",	 false},
		{"SHIFT",	 false},

		{"ENTER",	 false},

		{"PAUSE",	 false},
		{"QUIT",	 false},
	};

	for (const auto& action : this->actions) {
		this->callbacks[action.first] = std::vector<std::unique_ptr<IInteractable>>();
	}

	this->mouseX = 0;
	this->mouseY = 0;
}

void Input::setActionValue(const std::string& action, bool value) {
	this->actions[action] = value;
}

bool Input::getAction(const std::string& action) {
	return this->actions[action];
}

void Input::clear() {
	for (auto& pair : this->actions) {
		pair.second = false;
	}
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
		{SDLK_e, "INTERACT"},

		{SDLK_LSHIFT, "SHIFT"},

		{SDLK_RETURN, "ENTER"},

		{SDLK_ESCAPE, "PAUSE"},
	};
}

void InputSystem::collectInput() {
	SDL_GetMouseState(&this->input->mouseX, &this->input->mouseY);

	SDL_Event e;
	while (SDL_PollEvent(&e)) {
		switch (e.type) {
			case SDL_KEYDOWN: {
				auto action = this->keyActionMappings[e.key.keysym.sym];
				this->input->setActionValue(action, true);
				this->triggerCallbacks(action);

				break;
			}

			case SDL_KEYUP: {
				auto action = this->keyActionMappings[e.key.keysym.sym];
				this->input->setActionValue(action, false);

				break;
			}

			case SDL_QUIT: {
				this->input->setActionValue("QUIT", true);

				break;
			}

			case SDL_MOUSEBUTTONDOWN: {
				if (e.button.button == SDL_BUTTON_LEFT) {
					this->input->setActionValue("CLICK", true);
					this->triggerCallbacks("CLICK");
				}
				
				break;
			}


			case SDL_MOUSEBUTTONUP: {
				if (e.button.button == SDL_BUTTON_LEFT) {
					this->input->setActionValue("CLICK", false);
				}
			}
		}
	}
}

std::shared_ptr<Input> InputSystem::getInput() {
	return this->input;
}

void InputSystem::addEventCallback(std::unique_ptr<IInteractable>& callback) {
	this->input->callbacks[callback->getTrigger()].push_back(std::move(callback));
}

void InputSystem::checkPlayerInteractables(float playerX, float playerY, float playerW, float playerH) {
	auto& events = this->input->callbacks["INTERACT"];

	for (auto& e : events) {
		auto h = e->getHitbox();

		if (playerX + playerW > h->x && playerX < h->x + h->w
			&& playerY + playerH > h->y && playerY < h->y + h->h) {
			e->enable();
		} else {
			e->disable();
		}
	}
}

void InputSystem::triggerCallbacks(std::string event) {
	auto& events = this->input->callbacks[event];

	for (auto& e : events) {
		if (!e->isEnabled() || e->isDone())
			continue;

		e->call();
	}

	events.erase(
		std::remove_if(events.begin(), events.end(), [](auto& e) { return e->isDone(); }), events.end()
	);
}