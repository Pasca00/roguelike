#include "GlobalEffectManager.h"

void dealDamageFunc(std::shared_ptr<Movable>& target) {
	// auto health = target->getHealthComponent()
}

void healFunc(std::shared_ptr<Movable>& target) {
	// auto health = target->getHealthComponent()
}

void slowFunc(std::shared_ptr<Movable>& target) {
	// auto movable = target->getMovableComponent()
}

std::shared_ptr<StatusEffect> GlobalEffectManager::makeEffect(EffectName requested) {
	switch (requested) {
	case EffectName::BURN:
		return makeBurnEffect();

	case EffectName::FREEZE:

		break;

	case EffectName::SLOW:

		break;
	}
}

std::shared_ptr<StatusEffect> GlobalEffectManager::makeBurnEffect() {
	auto effect = std::make_shared<StatusEffect>(DURATION_LARGE, INTERVAL_NONE, dealDamageFunc);

	return effect;
}

float GlobalEffectManager::DURATION_LARGE = 5000.f;
float GlobalEffectManager::DURATION_MEDIUM = 2500.f;
float GlobalEffectManager::DURATION_SMALL = 1000.0;

float GlobalEffectManager::INTERVAL_LARGE = 1000.f;
float GlobalEffectManager::INTERVAL_SMALL = 500.f;
float GlobalEffectManager::INTERVAL_NONE = 0.f;