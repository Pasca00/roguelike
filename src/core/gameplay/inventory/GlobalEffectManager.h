#pragma once

#include "StatusEffect.h"

void dealDamageFunc(std::shared_ptr<Entity>& target) {
	//
}

class GlobalEffectManager {
private:

	static float DURATION_LARGE;
	static float DURATION_MEDIUM;
	static float DURATION_SMALL;

	static float INTERVAL_LARGE;
	static float INTERVAL_SMALL;
	static float INTERVAL_NONE;

public:
	static std::shared_ptr<StatusEffect> makeEffect(EffectName requested) {
		switch (requested) {
		case EffectName::BURN:
			return makeBurnEffect();

		case EffectName::FREEZE:
			
			break;

		case EffectName::SLOW:

			break;
		}
	}

private:
	static std::shared_ptr<StatusEffect> makeBurnEffect() {
		auto effect = std::make_shared<StatusEffect>(DURATION_LARGE, INTERVAL_NONE, dealDamageFunc);

		return effect;
	}
};

float GlobalEffectManager::DURATION_LARGE	= 5000.f;
float GlobalEffectManager::DURATION_MEDIUM	= 2500.f;
float GlobalEffectManager::DURATION_LARGE	= 1000.0;

float GlobalEffectManager::INTERVAL_LARGE	= 1000.f;
float GlobalEffectManager::INTERVAL_SMALL	= 500.f;
float GlobalEffectManager::INTERVAL_NONE	= 0.f;