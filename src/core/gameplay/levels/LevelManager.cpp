#include "LevelManager.h"

LevelManager::LevelManager(std::unique_ptr<IGenerator> generator) {
	this->generator = std::move(generator);
}

void LevelManager::generateTemplate() {
	auto layout = this->generator->generateLayout();
}