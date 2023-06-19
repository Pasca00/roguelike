#include "Combatable.h"
#include "../../systems/physics/Movable.h"


Combatable::Combatable(float rangeVertical, float rangeHorizontal, float maxHealth, float attackDamage) {
	this->rangeVertical = rangeVertical;
	this->rangeHorizontal = rangeHorizontal;

	this->maxHealth = maxHealth;
	this->currHealth = maxHealth;
	this->regen = 0.f;
	
	this->attackDamage = attackDamage;
	this->isAttacking = false;
	this->timeSinceLastHit = 50.f;
}

Combatable::~Combatable() {
	//printf("DECEMM???\n");
}

void Combatable::setInteractCallback(std::function<void(std::shared_ptr<Movable>&)> callback) {
	this->onInteract = callback;
}

void Combatable::setDamageTakenCallback(std::function<void(void)> callback) {
	this->onDamageTaken = callback;
}