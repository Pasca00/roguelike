#pragma once

#include <memory>
#include <functional>

#include "../Hitbox.h"

class Movable;

class Combatable {
public:
	float maxHealth;
	float currHealth;
	float regen;

	float attackDamage;

	//std::unique_ptr<Hitbox> combatHitbox;
	float rangeVertical;
	float rangeHorizontal;

	bool isAttacking;

	float timeSinceLastHit;
	bool recentlyDamaged;

	std::function<void(std::shared_ptr<Movable>&)> onInteract;
	std::function<void(void)> onDamageTaken;
	std::function<void(void)> onHitRecovery;
	std::function<void(void)> onAttack;
	std::function<void(void)> onDeath;

	Combatable(float rangeVertical, float rangeHorizontal, float maxHealth, float attackDamage);
	~Combatable();

	void setInteractCallback(std::function<void(std::shared_ptr<Movable>&)> callback);
	void setDamageTakenCallback(std::function<void(void)> callback);
};