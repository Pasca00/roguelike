#include "Movable.h"

Movable::Movable(std::shared_ptr<Hitbox>& hitbox, float acceleration) : hitbox(hitbox) {
	this->direction = glm::vec2(1, 0);
	this->velocity = glm::vec2(0, 0);

	this->acceleration = acceleration;

	this->currentAcceleration = 0.f;
	this->maxSpeed = 4.f;

	this->isMoving = false;

	this->doesCollide = true;
}

void Movable::setXDirection(float dir) {
	this->direction.x = dir;
}

void Movable::setYDirection(float dir) {
	this->direction.y = dir;
}

void Movable::startMovement() {
	this->isMoving = true;
}

void Movable::stopMovement() {
	this->isMoving = false;
}

void Movable::accelerate(float dtime) {
	if (this->isMoving) {
		this->velocity += this->acceleration;
		if (this->velocity.x > this->maxSpeed) {
			this->velocity.x = this->maxSpeed;
		}

		if (this->velocity.y > this->maxSpeed) {
			this->velocity.y = this->maxSpeed;
		}
	}
}

void Movable::applyFriction(float friction) {
	this->velocity -= friction;
	if (this->velocity.x < 0) {
		this->velocity.x = 0;
	}

	if (this->velocity.y < 0) {
		this->velocity.y = 0;
	}
}

void Movable::move(float dtime) {
	this->hitbox->x += this->velocity.x * this->direction.x;
	this->hitbox->y += this->velocity.y * this->direction.y;
}

void Movable::setCollision(bool collision) {
	this->doesCollide = collision;
}

bool Movable::collides() {
	return this->doesCollide;
}