#include "Combatable.h"
//#include "../../systems/physics/Movable.h"

Combatable::Combatable(float rangeVertical, float rangeHorizontal, float maxHealth, float attackDamage) {
	this->rangeVertical = rangeVertical;
	this->rangeHorizontal = rangeHorizontal;

	this->maxHealth = maxHealth;
	this->currHealth = maxHealth;
	this->regen = 0.f;
	
	this->attackDamage = attackDamage;
	this->isAttacking = false;
	this->timeSinceLastHit = 50.f;

	this->recentlyDamaged = false;

	this->critChance = 0.f;

	this->feared = 0;
}

float Combatable::getDamageDealt() {
	if (rand() % 100 <= this->critChance) {
		return this->attackDamage * 2;
	}

	return this->attackDamage;
}

Combatable::~Combatable() {
	//printf("DECEMM???\n");
}
