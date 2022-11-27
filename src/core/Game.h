#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "states/IState.h"
#include "states/MainMenuState.h"

class Game {
private:
	static Game* instance;

	std::shared_ptr<VideoSystem> videoSystem;
	std::shared_ptr<InputSystem> inputSystem;
	std::shared_ptr<PhysicsSystem> physicsSystem;

	std::unique_ptr<IState> currentState;

	Game();

	void createSystems();
	void initSystems();

public:

	static Game*& getInstance();

	bool shouldRun();

	void collectInput();
	void update();
	void draw();
	void clearScreen();

	void swapWindow();

	~Game();
};