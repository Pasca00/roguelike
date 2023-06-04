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
	for (auto& m : this->movables) {
		m->applyFriction(this->friction * dtime);
		m->accelerate(dtime);
	
		if (m->collides()) {
			this->computeMovablePosition(m);
		} else {
			m->move(dtime);
		}
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

void PhysicsSystem::setMap(std::vector<std::vector<std::shared_ptr<Tile>>>& tilemap, TreeNode* mapTree, float tileSize) {
	this->currentTilemap = tilemap;
	this->currentMapTree = mapTree;
	this->tileSize = tileSize;
}

void PhysicsSystem::computeMovablePosition(std::shared_ptr<Movable>& m) {
	auto& mHitbox = m->hitbox;

	float potentialX = mHitbox->x + m->velocity.x * m->direction.x;
	float potentialY = mHitbox->y + m->velocity.y * m->direction.y;

	int currTileX = mHitbox->x / this->tileSize;
	int currTileY = this->currentTilemap.size() - 1 - mHitbox->y / this->tileSize;
	
	auto tileLeft	= this->currentTilemap[currTileY][currTileX - 1];
	auto tileRight	= this->currentTilemap[currTileY][currTileX + 1];
	auto tileUp		= this->currentTilemap[currTileY][currTileX];
	auto tileDown	= this->currentTilemap[currTileY + 1][currTileX];

	if (m->direction.x == -1) {
		if (tileLeft->type != IGenerator::WALL) {
			mHitbox->x = potentialX;
		} else {
			auto& view = tileLeft->getView();
			if (potentialX < view->getX() + view->getWidth() * view->getSize()) {
				mHitbox->x = view->getX() + view->getWidth() * view->getSize();
			} else {
				mHitbox->x = potentialX;
			}
		}
	} else if (m->direction.x == 1) {
		if (tileRight->type != IGenerator::WALL) {
			mHitbox->x = potentialX;
		} else {
			auto& view = tileRight->getView();
			if (potentialX + mHitbox->w > view->getX()) {
				mHitbox->x = view->getX() - mHitbox->w;
			} else {
				mHitbox->x = potentialX;
			}
		}
	}

	if (m->direction.y == -1) {
		if (tileDown->type != IGenerator::WALL) {
			mHitbox->y = potentialY;
		} else {
			auto& view = tileDown->getView();
			if (potentialY < view->getY() + view->getHeight() * view->getSize()) {
				mHitbox->y = view->getY() + view->getHeight() * view->getSize();
			} else {
				mHitbox->y = potentialY;
			}
		}
	} else if (m->direction.y == 1) {
		if (tileUp->type != IGenerator::WALL) {
			mHitbox->y = potentialY;
		} else {
			auto& view = tileUp->getView();
			if (potentialY + mHitbox->h > view->getY()) {
				mHitbox->y = view->getY() - mHitbox->h;
			} else {
				mHitbox->y = potentialY;
			}
		}
	}
}
	
float PhysicsSystem::getFrameDeltaTime() {
	return dTime;
}

unsigned int PhysicsSystem::getTotalTime() {
	return SDL_GetTicks();
}