#pragma once

#include "EffectName.h"
#include "StatusEffect.h"

class GlobalEffectManager {
public:
	static float DURATION_LARGE;
	static float DURATION_MEDIUM;
	static float DURATION_SMALL;
	static float DURATION_NONE;

	static float INTERVAL_LARGE;
	static float INTERVAL_SMALL;
	static float INTERVAL_NONE;

public:
	static std::shared_ptr<StatusEffect> makeEffect(EffectName requested);

private:
	static std::shared_ptr<StatusEffect> makeBurnEffect();
	static std::shared_ptr<StatusEffect> makeSlowEffect();
	static std::shared_ptr<StatusEffect> makeTimeFreezeEffect();
	static std::shared_ptr<StatusEffect> makeBleedEffect();
	static std::shared_ptr<StatusEffect> makeFearEffect();
};