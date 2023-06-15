#pragma once

#include <fstream>
#include <string>
#include <unordered_map>
#include <memory>

#include <SDL.h>

#include "../ISystem.h"
#include "../../Paths.h"
#include "IInteractable.h"

struct Input {
	std::unordered_map<std::string, bool> actions;
	std::unordered_map<std::string, std::vector<std::unique_ptr<IInteractable>>> callbacks;

	int mouseX;
	int mouseY;

	Input();

	void setActionValue(const std::string& action, bool value);
	bool getAction(const std::string& action);

	void clear();
};

class InputSystem : public ISystem {
private:
	static const std::string keyMappingsFilename;

	std::unordered_map<SDL_Keycode, std::string> keyActionMappings;
	std::shared_ptr<Input> input;

	bool settingsFileExists();
	void createDefaultKeyMappings();
	void readKeyMappings();

	void triggerCallbacks(std::string action);

public:
	InputSystem();

	void init();

	void collectInput();

	std::shared_ptr<Input> getInput();

	void addEventCallback(std::unique_ptr<IInteractable>& callback);

	void checkPlayerInteractables(float playerX, float playerY, float playerW, float playerH);
};