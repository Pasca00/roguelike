#pragma once

#include "EffectName.h"
#include "StatusEffect.h"

class GlobalEffectManager {
private:
	static float DURATION_LARGE;
	static float DURATION_MEDIUM;
	static float DURATION_SMALL;

	static float INTERVAL_LARGE;
	static float INTERVAL_SMALL;
	static float INTERVAL_NONE;

public:
	static std::shared_ptr<StatusEffect> makeEffect(EffectName requested);

private:
	static std::shared_ptr<StatusEffect> makeBurnEffect();
};