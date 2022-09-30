#include "Game.h"

#include <iostream>

Game* Game::instance = nullptr;

Game*& Game::getInstance() {
	if (Game::instance == nullptr) {
		Game::instance = new Game();
	}

	return Game::instance;
}

Game::Game() {}

Game::~Game() {
	std::cout << "Destroyed\n";
}

bool Game::shouldRun() {
	return true;
}

void Game::draw() {
	std::cout << "rendering\n";
}