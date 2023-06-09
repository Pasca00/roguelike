#pragma once

struct ControllableParameters {
	float& physicsTimeModifier;
	float& playerMaxSpeed;
	
	// float& playerAttackPower;
	
	bool& playerCollisionIsEnabled;

	ControllableParameters(
		float& physicsTimeModifier,
		float& playerMaxSpeed,
		bool& playerCollisionIsEnabled
	) : physicsTimeModifier(physicsTimeModifier),
		playerMaxSpeed(playerMaxSpeed),
		playerCollisionIsEnabled(playerCollisionIsEnabled) { }

	void slowTimeModifier() {
		this->physicsTimeModifier = 0.5f;
	}

	void restoreTimeModifier() {
		this->physicsTimeModifier = 1.0f;
	}
};