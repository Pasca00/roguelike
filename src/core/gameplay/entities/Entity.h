#pragma once

#include "../../systems/input/InputSystem.h"
#include "../../systems/video/views/AnimationView.h"
#include "../../systems/physics/Movable.h"

#include "../inventory/GlobalEffectManager.h"

#define ENTITY_ATTACK_FRAME 5

enum class EntityState {
	DORMANT, WAKING, IDLE, ATTACK, MOVE, DYING, DEAD
};

class Entity {
protected:
	std::shared_ptr<Movable> movableComponent;

	EntityState currentState;

	std::shared_ptr<AnimationView> animation;

	std::vector<std::shared_ptr<AnimationView>> idleAnimations;
	std::vector<std::shared_ptr<AnimationView>> attackAnimations;
	std::vector<std::shared_ptr<AnimationView>> moveAnimations;
	std::shared_ptr<AnimationView> deadAnimation;

	virtual void switchState(EntityState to);

	bool drawFlipped;

	bool enabled;

	int attackFrame;

public:
	Entity( // TODO: change these to pairs or maps
		std::shared_ptr<Movable>& movableComponent,
		std::vector<std::shared_ptr<AnimationView>>& idleAnimations,
		std::vector<std::shared_ptr<AnimationView>>& attackAnimations,
		std::vector<std::shared_ptr<AnimationView>>& moveAnimations,
		std::shared_ptr<AnimationView>& deadAnimation,
		int attackFrame
	);

	virtual void handleInput(std::shared_ptr<Input>& input);
	virtual void update(float dtime);

	std::shared_ptr<AnimationView> getCurrentTexture();

	std::shared_ptr<Movable>& getMovableComponent();

	void addOnHitEffect(EffectName effectName);
	void addStatusEffect(EffectName effectName);

	virtual void interactWithEnemy(std::shared_ptr<Movable>& m);

	void enable();
	void disable();
	bool isEnabled();

	EntityState getCurrentState();
};