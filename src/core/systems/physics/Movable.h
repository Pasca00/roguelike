#pragma once

#include "../../../glm/glm.hpp"

class Movable {
public:
	glm::vec2 velocity;
	glm::vec2 direction;
	glm::vec2 position;

	float currentAcceleration;
	float maxSpeed;
	float acceleration;

	bool isMoving;

public:
	Movable(glm::vec2& position, float acceleration = 2.5f);

	void setXDirection(float dir);
	void setYDirection(float dir);

	void startMovement();
	void stopMovement();

	void accelerate(float dtime = 1.0f);
	void applyFriction(float friction);
	void move(float dtime);
};