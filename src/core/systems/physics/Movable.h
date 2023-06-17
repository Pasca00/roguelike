#pragma once

#include <memory>

#include "../../../glm/glm.hpp"
#include "../../gameplay/Hitbox.h"

class Movable {
public:
	std::shared_ptr<Hitbox> hitbox;

	glm::vec2 velocity;
	glm::vec2 direction;
	//glm::vec2 position;

	float currentAcceleration;
	float maxSpeed;
	float acceleration;

	bool isMoving;

	bool doesCollide;

	float dashCooldown;
	float timeSinceDash;

	int currentlyFacingX;

public:
	Movable(std::shared_ptr<Hitbox>& hitbox, float acceleration = 2.5f);

	void setXDirection(float dir);
	void setYDirection(float dir);

	void startMovement();
	void stopMovement();

	void accelerate(float dtime = 1.0f);
	void applyFriction(float friction, float dtime = 1.0f);
	void move(float dtime);

	void setCollision(bool collision);
	bool collides();

	void dash();
};