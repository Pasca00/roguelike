#include "PhysicsSystem.h"

PhysicsSystem::PhysicsSystem() { }

void PhysicsSystem::init() {
	this->totalTime = 0;
	this->currTime = 0;
	this->prevTime = 0;
	this->dTime = 0;

	this->friction = 120.f;

	this->timeModifier = 1.0;
}

void PhysicsSystem::update(float dtime) {
	for (auto& m : this->movables) {
		m->applyFriction(this->friction);
		m->accelerate();
	
		if (m->collides()) {
			this->computeMovablePosition(m, dtime);
		} else {
			m->move(dtime);
		}
	}

	this->checkPlayerMovablesInteractions();
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

void PhysicsSystem::computeMovablePosition(std::shared_ptr<Movable>& m, float dTime) {
	auto& mHitbox = m->hitbox;

	float potentialX = mHitbox->x + m->velocity.x * m->direction.x * dTime;
	float potentialY = mHitbox->y + m->velocity.y * m->direction.y * dTime;

	if (m->velocity.x == 0 && m->velocity.y == 0) {
		return;
	}

	int currTileX = mHitbox->x / this->tileSize;
	int currTileY = this->currentTilemap.size() - mHitbox->y / this->tileSize;
	
	auto currTile		= this->currentTilemap[currTileY][currTileX];

	auto tileLeft		= this->currentTilemap[currTileY][currTileX - 1];
	auto tileRight		= this->currentTilemap[currTileY][currTileX + 1];

	auto tileUp			= this->currentTilemap[currTileY - 1][currTileX];
	auto tileDown		= this->currentTilemap[currTileY + 1][currTileX];

	auto tileUpperLeft	= this->currentTilemap[currTileY - 1][currTileX - 1];
	auto tileUpperRight = this->currentTilemap[currTileY - 1][currTileX + 1];
	auto tileLowerRight = this->currentTilemap[currTileY + 1][currTileX + 1];

	bool checkedDown = false;
	bool checkedLeft = false;

	if (currTile->type == IGenerator::DOOR_HORIZONTAL) {
		auto& view = currTile->getView();
		if (potentialX < view->getX() + 16.f) {
			mHitbox->x = view->getX() + 16.f;

			checkedLeft = true;
		}
	} else if (currTile->type == IGenerator::DOOR_VERTICAL) {
		auto& view = currTile->getView();
		if (potentialY < view->getY() + 16.f) {
			mHitbox->y = view->getY() + 16.f;
		
			checkedDown = true;
		}
	}

	if (m->direction.x == -1 && !checkedLeft) {
		if (tileLeft->type != IGenerator::WALL) {
			if (tileUpperLeft->type != IGenerator::WALL) {
				mHitbox->x = potentialX;
			} else {
				auto& view = tileUpperLeft->getView();
				if (mHitbox->y + mHitbox->h > view->getY() && potentialX < view->getX() + tileSize) {
					mHitbox->x = view->getX() + tileSize;
				} else {
					mHitbox->x = potentialX;
				}
			}
		} else {
			auto& view = tileLeft->getView();
			if (potentialX < view->getX() + tileSize) {
				mHitbox->x = view->getX() + tileSize;
			} else {
				mHitbox->x = potentialX;
			}
		}
	} else if (m->direction.x == 1) {
		if (tileRight->type != IGenerator::WALL && tileRight->type != IGenerator::DOOR_HORIZONTAL) {
			if (tileUpperRight->type != IGenerator::WALL) {
				mHitbox->x = potentialX;
			} else {
				auto& view = tileUpperRight->getView();
				if (mHitbox->y + mHitbox->h > view->getY() && potentialX + mHitbox->w > view->getX()) {
					mHitbox->x = view->getX() - mHitbox->w;
				} else {
					mHitbox->x = potentialX;
				}
			}
		} else {
			auto& view = tileRight->getView();
			if (potentialX + mHitbox->w > view->getX()) {
				mHitbox->x = view->getX() - mHitbox->w;
			} else {
				mHitbox->x = potentialX;
			}
		}
	}

	if (m->direction.y == -1 && !checkedDown) {
		if (tileDown->type != IGenerator::WALL) {
			if (tileLowerRight->type != IGenerator::WALL) {
				mHitbox->y = potentialY;
			} else {
				auto& view = tileLowerRight->getView();
				if (mHitbox->x + mHitbox->w > view->getX() && potentialY <= view->getY() + tileSize) {
					mHitbox->y = view->getY() + tileSize + 0.1f;
				} else {
					mHitbox->y = potentialY;
				}
			}
		} else {
			auto& view = tileDown->getView();
			if (potentialY <= view->getY() + tileSize) {
				mHitbox->y = view->getY() + tileSize + 0.1f;
			} else {
				mHitbox->y = potentialY;
			}
		}
	} else if (m->direction.y == 1) {
		if (tileUp->type != IGenerator::WALL && tileUp->type != IGenerator::DOOR_VERTICAL) {
			if (tileUpperRight->type != IGenerator::WALL) {
				mHitbox->y = potentialY;
			} else {
				auto& view = tileUpperRight->getView();
				if (mHitbox->x + mHitbox->w > view->getX() && potentialY + mHitbox->h > view->getY()) {
					mHitbox->y = view->getY() - mHitbox->h;
				} else {
					mHitbox->y = potentialY;
				}
			}
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

void PhysicsSystem::checkPlayerMovablesInteractions() {
	// Player is always second in the movable list
	auto& player = this->movables[1];

	int iPlayer = this->currentTilemap.size() - 1 - player->hitbox->y / this->tileSize;
	int jPlayer = player->hitbox->x / this->tileSize;

	auto room = TreeNode::findRoomForCoords(this->currentMapTree, iPlayer, jPlayer);

	for (int i = 2; i < this->movables.size(); i++) {
		auto& m = this->movables[i];

		int iMov = this->currentTilemap.size() - 1 - m->hitbox->y / this->tileSize;
		int jMov = m->hitbox->x / this->tileSize;

		// Only check interaction for enemies in the same room
		if (iMov >= room->y && iMov < room->h && jMov >= room->x && jMov < room->w) {

			if (m->hitbox->x <= player->hitbox->x + player->hitbox->w
				&& m->hitbox->x + m->hitbox->w >= player->hitbox->x
				&& m->hitbox->y <= player->hitbox->y + player->hitbox->h
				&& m->hitbox->y + m->hitbox->h >= player->hitbox->y) {
				// TODO
			}
		}
	}
}
	
float PhysicsSystem::getFrameDeltaTime() {
	return dTime * this->timeModifier;
}

unsigned int PhysicsSystem::getTotalTime() {
	return SDL_GetTicks();
}

float& PhysicsSystem::getTimeModifier() {
	return this->timeModifier;
}