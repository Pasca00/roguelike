#include "Movable.h"

#define FACING_NONE  0
#define FACING_LEFT  1
#define FACING_RIGHT 2
#define FACING_UP	 3
#define FACING_DOWN	 4

Movable::Movable(
	std::shared_ptr<Hitbox>& hitbox, 
	std::shared_ptr<Combatable>& combatableComponent, 
	float acceleration
) : hitbox(hitbox), combatableComponent(combatableComponent) {
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

	this->disabled = false;
}

void Movable::setXDirection(float dir) {
	this->direction.x = dir;
	this->currentlyFacingX = dir != 0 ? dir : currentlyFacingX;
}

void Movable::setYDirection(float dir) {
	this->direction.y = dir;
}

void Movable::setCombatableComponent(std::shared_ptr<Combatable>& combatableComponent) {
	this->combatableComponent = combatableComponent;
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

void Movable::push(int direction) {
	this->velocity.x = -200;
	this->velocity.y = -200;
}

void Movable::update(float dtime) {
	this->timeSinceDash += dtime;
	if (combatableComponent->timeSinceLastHit < 50.f) {
		this->combatableComponent->timeSinceLastHit += dtime;
		if (combatableComponent->timeSinceLastHit >= 50.f) {
			this->combatableComponent->onHitRecovery();
		}
	}

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

void Movable::interactWith(std::shared_ptr<Movable>& m) {
	if (this->combatableComponent->isAttacking 
		&& m->combatableComponent->timeSinceLastHit >= 50.f
		&& this->combatableComponent->onInteract != nullptr) {
		m->combatableComponent->timeSinceLastHit = 0.f;
		//m->push(this->currentlyFacingX);
		this->combatableComponent->onInteract(m);
	}

	return;
}