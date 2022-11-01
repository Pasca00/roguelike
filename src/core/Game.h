#pragma once

#include <iostream>
#include <memory>
#include <vector>

#include "systems/video/VideoSystem.h"

class Game {
private:
	static Game* instance;

	std::shared_ptr<VideoSystem> videoSystem;

	Game();

	void initSystems();

public:

	static Game*& getInstance();

	bool shouldRun();

	void draw();

	~Game();
};