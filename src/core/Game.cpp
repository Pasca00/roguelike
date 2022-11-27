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

	this->currentState = std::make_unique<MainMenuState>(this->inputSystem, this->videoSystem, this->physicsSystem);
}

void Game::createSystems() {
	videoSystem = std::make_shared<VideoSystem>();
	inputSystem = std::make_shared<InputSystem>();
	physicsSystem = std::make_shared<PhysicsSystem>();
}

void Game::initSystems() {
	std::vector<std::shared_ptr<ISystem>> systems = {
		videoSystem,
		inputSystem,
		physicsSystem,
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

void Game::update() {
	this->physicsSystem->computeFrameDeltaTime();
	this->currentState->update(this->physicsSystem->getFrameDeltaTime());
}

void Game::draw() {
	// TODO: make render() call to the state
	this->currentState->render();
}

void Game::clearScreen() {
	videoSystem->clearScreen();
}

void Game::swapWindow() {
	this->videoSystem->swapWindow();
}