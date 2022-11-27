#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() { }

void PhysicsSystem::init() {
	this->currTime = 0;
	this->prevTime = 0;
	this->dTime = 0;
}

void PhysicsSystem::computeFrameDeltaTime() {
	currTime = SDL_GetPerformanceCounter();
	this->dTime = static_cast<float>(
		(currTime - prevTime) / static_cast<double>(SDL_GetPerformanceFrequency())
	);
	this->prevTime = currTime;
}

float PhysicsSystem::getFrameDeltaTime() {
	return dTime;
}