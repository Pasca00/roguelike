#include "Game.h"

#include <iostream>

Game* Game::instance = nullptr;

Game*& Game::getInstance() {
	if (Game::instance == nullptr) {
		Game::instance = new Game();
	}

	return Game::instance;
}

Game::Game() {
	this->createSystems();
	this->initSystems();

	this->currentState = std::make_unique<MainMenuState>(this->inputSystem, this->videoSystem);
}

void Game::createSystems() {
	videoSystem = std::make_shared<VideoSystem>();
	inputSystem = std::make_shared<InputSystem>();
}

void Game::initSystems() {
	std::vector<std::shared_ptr<ISystem>> systems = {
		videoSystem,
		inputSystem,
	};

	for (const auto& system : systems) {
		system->init();
	}
}

Game::~Game() {
	std::cout << "Destroyed\n";
}

bool Game::shouldRun() {
	return true;
}

void Game::collectInput() {
	this->inputSystem->collectInput();
}

void Game::draw() {
	std::cout << "rendering\n";
}

void Game::clearScreen() {
	videoSystem->clearScreen();
}