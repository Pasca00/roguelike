#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() { }

void PhysicsSystem::init() {
	this->totalTime = 0;
	this->currTime = 0;
	this->prevTime = 0;
	this->dTime = 0;

	this->friction = 12.f;
}

void PhysicsSystem::update(float dtime) {
	for (auto m : this->movables) {
		m->applyFriction(this->friction * dtime);
		m->accelerate(dtime);

		m->move(dtime);
	}
}

void PhysicsSystem::addMovable(std::shared_ptr<Movable>& movable) {
	this->movables.push_back(movable);
}

void PhysicsSystem::computeFrameDeltaTime() {
	currTime = SDL_GetPerformanceCounter();
	this->dTime = static_cast<float>(
		(currTime - prevTime) / static_cast<double>(SDL_GetPerformanceFrequency())
	);
	this->prevTime = currTime;

	this->totalTime += this->dTime;
}

void PhysicsSystem::setMap(std::vector<std::vector<std::shared_ptr<Tile>>>& tilemap, TreeNode* mapTree) {
	this->currentTilemap = tilemap;
	this->currentMapTree = mapTree;
}

float PhysicsSystem::getFrameDeltaTime() {
	return dTime;
}

unsigned int PhysicsSystem::getTotalTime() {
	return SDL_GetTicks();
}