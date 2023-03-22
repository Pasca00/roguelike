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

	this->stateChange = false;
	this->stateChangeTimeRequired = 5.0;
	this->stateChangeTime = 0.0;

	this->currentState = std::make_unique<MainMenuState>(
		this->stateChange,
		this->inputSystem, 
		this->videoSystem, 
		this->physicsSystem,
		this->soundSystem,
		this->generalSystem
	);
}

void Game::createSystems() {
	videoSystem = std::make_shared<VideoSystem>();
	inputSystem = std::make_shared<InputSystem>();
	physicsSystem = std::make_shared<PhysicsSystem>();
	soundSystem = std::make_shared<SoundSystem>();
	generalSystem = std::make_shared<GeneralSystem>();
}

void Game::initSystems() {
	std::vector<std::shared_ptr<ISystem>> systems = {
		videoSystem,
		inputSystem,
		physicsSystem,
		soundSystem,
		generalSystem,
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
	float dTime = this->physicsSystem->getFrameDeltaTime();

	if (this->stateChange) {
		this->stateChangeTime += dTime;
		if (this->stateChangeTime >= stateChangeTimeRequired) {
			this->stateChangeTime = 0;
			this->stateChange = false;

			this->generalSystem->queueThreadJob([this] {
				this->currentState = std::make_unique<PlayingState>(
					this->stateChange,
					this->inputSystem,
					this->videoSystem,
					this->physicsSystem,
					this->soundSystem,
					this->generalSystem
				);
			});
			
		}
	}

	this->videoSystem->updateTransition(dTime);
	this->currentState->update(dTime);
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