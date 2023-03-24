#pragma once

#include <memory>

#include "IGenerator.h"
#include "BSPGenerator.h"

class LevelManager {
private:
	std::unique_ptr<IGenerator> generator;


public:
	LevelManager(std::unique_ptr<IGenerator> generator);
	void generateTemplate();
};