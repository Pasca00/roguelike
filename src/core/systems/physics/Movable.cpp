#include "Movable.h"

#define FACING_NONE  0
#define FACING_LEFT  1
#define FACING_RIGHT 2
#define FACING_UP	 3
#define FACING_DOWN	 4

Movable::Movable(std::shared_ptr<Hitbox>& hitbox, float acceleration) : hitbox(hitbox) {
	this->direction = glm::vec2(1, 0);
	this->velocity = glm::vec2(0, 0);

	this->acceleration = acceleration;

	this->currentAcceleration = 0.f;
	this->maxSpeed = 200.f;

	this->isMoving = false;

	this->doesCollide = true;

	this->dashCooldown = 50.f;
	this->timeSinceDash = 50.f;

	this->currentlyFacingX = FACING_RIGHT;
}

void Movable::setXDirection(float dir) {
	this->direction.x = dir;
	this->currentlyFacingX = dir != 0 ? dir : currentlyFacingX;
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

void Movable::dash() {
	if (this->timeSinceDash < this->dashCooldown) {
		return;
	}

	this->timeSinceDash = 0.f;

	this->velocity.x = 1000.f;
	this->velocity.y = 1000.f;

	if (this->direction.x == 0) {
		this->direction.x = this->currentlyFacingX;
	}
}

void Movable::accelerate(float dtime) {
	this->timeSinceDash += dtime;

	if (this->isMoving) {
		if (this->velocity.x < this->maxSpeed) {
			this->velocity.x += this->acceleration * dtime;
			if (this->velocity.x > this->maxSpeed) {
				this->velocity.x = this->maxSpeed;
			}
		}

		if (this->velocity.y < this->maxSpeed) {
			this->velocity.y += this->acceleration * dtime;
			if (this->velocity.y > this->maxSpeed) {
				this->velocity.y = this->maxSpeed;
			}
		}
	}
}

void Movable::applyFriction(float friction, float dtime) {
	this->velocity -= friction * dtime;
	if (this->velocity.x < 0) {
		this->velocity.x = 0;
	}

	if (this->velocity.y < 0) {
		this->velocity.y = 0;
	}
}

void Movable::move(float dtime) {
	this->hitbox->x += this->velocity.x * this->direction.x * dtime;
	this->hitbox->y += this->velocity.y * this->direction.y * dtime;
}

void Movable::setCollision(bool collision) {
	this->doesCollide = collision;
}

bool Movable::collides() {
	return this->doesCollide;
}