#pragma once

class Game {
private:
	static Game* instance;

public:

	static Game* getInstance();

	bool shouldRun();

	void draw();
};