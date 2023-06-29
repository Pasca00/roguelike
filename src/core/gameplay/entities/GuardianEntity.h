#pragma once

#include "Entity.h"

class GuardianEntity : public Entity {
private:
	std::shared_ptr<AnimationView> dormantAnimation;
	std::shared_ptr<AnimationView> wakingAnimation;

	void switchState(EntityState to) override;

public:
	GuardianEntity( // TODO: change these to pairs or maps
		std::shared_ptr<Movable>& movableComponent,
		std::shared_ptr<AnimationView>& dormantAnimation,
		std::shared_ptr<AnimationView>& wakingAnimation,
		std::vector<std::shared_ptr<AnimationView>>& idleAnimations,
		std::vector<std::shared_ptr<AnimationView>>& attackAnimations,
		std::vector<std::shared_ptr<AnimationView>>& moveAnimations,
		std::shared_ptr<AnimationView>& deadAnimation,
		int attackFrame
	);

	void handleInput(std::shared_ptr<Input>& input) override;
	void update(float dtime) override;
};