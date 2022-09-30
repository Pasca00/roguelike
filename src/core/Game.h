#pragma once

#include <memory>
#include <iostream>

class Game {
private:
	static Game* instance;

	Game();

public:

	static Game*& getInstance();

	bool shouldRun();

	void draw();

	~Game();
};