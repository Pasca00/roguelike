#include "GlobalEffectManager.h"

#include "../../systems/physics/Movable.h"

void dealDamageFunc(std::shared_ptr<Movable>& target) {
	target->combatableComponent->currHealth -= 5.f;
	target->combatableComponent->onDamageTaken();
}

void healFunc(std::shared_ptr<Movable>& target) {
	// auto health = target->getHealthComponent()
}

void slowFunc(std::shared_ptr<Movable>& target) {
	// auto movable = target->getMovableComponent()
	target->maxSpeed = 0.5 * target->maxSpeed;
}

void timeFreezeFunc(std::shared_ptr<Movable>& target) {
	target->timeModifier = 0;
	target->disabled = true;
}

void timeFreezeReverseFunc(std::shared_ptr<Movable>& target) {
	target->timeModifier = 1.f;
	target->disabled = false;
}

void removeSlowFunc(std::shared_ptr<Movable>& target) {
	target->maxSpeed *= 2;
}

void addFearFunc(std::shared_ptr<Movable>& target) {
	target->combatableComponent->feared = true;
}

void removeFearFunc(std::shared_ptr<Movable>& target) {
	target->combatableComponent->feared = false;
}

std::shared_ptr<StatusEffect> GlobalEffectManager::makeEffect(EffectName requested) {
	switch (requested) {
	case EffectName::BURN:
		return makeBurnEffect();

	case EffectName::TIME_RIFT:
		return makeTimeFreezeEffect();
		
	case EffectName::SLOW:
		return makeSlowEffect();

	case EffectName::BLEED:
		return makeBleedEffect();

	case EffectName::FEAR:
		return makeFearEffect();
	}
}

std::shared_ptr<StatusEffect> GlobalEffectManager::makeBurnEffect() {
	auto effect = std::make_shared<StatusEffect>(DURATION_LARGE, INTERVAL_SMALL, dealDamageFunc);

	return effect;
}

std::shared_ptr<StatusEffect> GlobalEffectManager::makeSlowEffect() {
	auto effect = std::make_shared<StatusEffect>(DURATION_LARGE, INTERVAL_NONE, slowFunc, removeSlowFunc);

	return effect;
}

std::shared_ptr<StatusEffect> GlobalEffectManager::makeTimeFreezeEffect() {
	auto effect = rand() % 100 < 10 ? std::make_shared<StatusEffect>(DURATION_SMALL, INTERVAL_NONE, timeFreezeFunc, timeFreezeReverseFunc)
								    : nullptr;

	return effect;
}

std::shared_ptr<StatusEffect> GlobalEffectManager::makeBleedEffect() {
	auto effect = rand() % 100 < 10 ? std::make_shared<StatusEffect>(DURATION_NONE, INTERVAL_LARGE, dealDamageFunc)
									: nullptr;

	return effect;
}

std::shared_ptr<StatusEffect> GlobalEffectManager::makeFearEffect() {
	auto effect = rand() % 100 < 12 ? std::make_shared<StatusEffect>(DURATION_MEDIUM, INTERVAL_NONE, addFearFunc, removeFearFunc)
									: nullptr;

	return effect;
}

float GlobalEffectManager::DURATION_NONE = 1000.f;
float GlobalEffectManager::DURATION_LARGE = 10.f;
float GlobalEffectManager::DURATION_MEDIUM = 7.5f;
float GlobalEffectManager::DURATION_SMALL = 5.f;

float GlobalEffectManager::INTERVAL_LARGE = 2.5f;
float GlobalEffectManager::INTERVAL_SMALL = 1.f;
float GlobalEffectManager::INTERVAL_NONE = 0.f;