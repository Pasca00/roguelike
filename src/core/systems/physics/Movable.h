#pragma once

#include <memory>

#include "../../../glm/glm.hpp"
#include "../../gameplay/Hitbox.h"
#include "../../gameplay/entities/Combatable.h"
#include "../../gameplay/inventory/GlobalEffectManager.h"

class Movable {
public:
	std::shared_ptr<Hitbox> hitbox;
	std::shared_ptr<Combatable> combatableComponent;

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

	bool disabled;

	std::function<void(std::shared_ptr<Movable>&)> onInteract;

	std::vector<EffectName> onHitApplies;
	std::vector<std::shared_ptr<StatusEffect>> activeStatusEffects;

public:
	Movable(std::shared_ptr<Hitbox>& hitbox, std::shared_ptr<Combatable>& combatableComponent, float acceleration = 2.5f);

	void setXDirection(float dir);
	void setYDirection(float dir);

	void startMovement();
	void stopMovement();

	void update(float dtime = 1.0f);
	void applyFriction(float friction, float dtime = 1.0f);
	void move(float dtime);

	void setCollision(bool collision);
	bool collides();

	void dash();
	void push(int direction);

	void interactWith(std::shared_ptr<Movable>& m);
	void setCombatableComponent(std::shared_ptr<Combatable>& combatableComponent);

	void addOnHitEffect(EffectName effectName);
	void addStatusEffect(EffectName effectName);

	//void addStatusEffect(EffectName effectName);
};