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

	this->currentState = std::make_unique<MainMenuState>(
		this->inputSystem, 
		this->videoSystem, 
		this->physicsSystem,
		this->soundSystem
	);
}

void Game::createSystems() {
	videoSystem = std::make_shared<VideoSystem>();
	inputSystem = std::make_shared<InputSystem>();
	physicsSystem = std::make_shared<PhysicsSystem>();
	soundSystem = std::make_shared<SoundSystem>();
}

void Game::initSystems() {
	std::vector<std::shared_ptr<ISystem>> systems = {
		videoSystem,
		inputSystem,
		physicsSystem,
		soundSystem,
	};

	for (const auto& system : systems) {
		system->init();
	}
}

Game::~Game() {
	std::cout << "Destroyed\n";
}

bool Game::shouldRun() {
	return !this->inputSystem->getInput()->getAction("QUIT");
}

void Game::collectInput() {
	this->inputSystem->collectInput();
}

void Game::update() {
	this->physicsSystem->computeFrameDeltaTime();
	this->currentState->update(this->physicsSystem->getFrameDeltaTime());

	this->videoSystem->updateTransition(this->physicsSystem->getFrameDeltaTime());
}

void Game::draw() {
	this->currentState->render();
	this->videoSystem->drawTransition();
}

void Game::clearScreen() {
	videoSystem->clearScreen();
}

void Game::swapWindow() {
	this->videoSystem->swapWindow();
}

float Game::getFrameTime() {
	return this->physicsSystem->getFrameDeltaTime();
}