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
}

Combatable::~Combatable() {
	//printf("DECEMM???\n");
}
