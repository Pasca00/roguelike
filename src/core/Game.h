#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "systems/video/VideoSystem.h"
#include "systems/input/InputSystem.h"

class Game {
private:
	static Game* instance;

	std::shared_ptr<VideoSystem> videoSystem;
	std::shared_ptr<InputSystem> inputSystem;

	Game();

	void createSystems();
	void initSystems();

public:

	static Game*& getInstance();

	bool shouldRun();

	void collectInput();
	void draw();

	~Game();
};