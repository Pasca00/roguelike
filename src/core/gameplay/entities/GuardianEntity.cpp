#include "GuardianEntity.h"

GuardianEntity::GuardianEntity( // TODO: change these to pairs or maps
	std::shared_ptr<Movable>& movableComponent,
	std::shared_ptr<AnimationView>& dormantAnimation,
	std::shared_ptr<AnimationView>& wakingAnimation,
	std::vector<std::shared_ptr<AnimationView>>& idleAnimations,
	std::vector<std::shared_ptr<AnimationView>>& attackAnimations,
	std::vector<std::shared_ptr<AnimationView>>& moveAnimations,
	std::shared_ptr<AnimationView>& deadAnimation,
	int attackFrame
) : Entity(
		movableComponent, 
		idleAnimations, 
		attackAnimations, 
		moveAnimations, 
		deadAnimation, 
		attackFrame
	),
	dormantAnimation(dormantAnimation), 
	wakingAnimation(wakingAnimation)  { 

	this->currentState = EntityState::DORMANT;
	this->animation = dormantAnimation;
}

void GuardianEntity::handleInput(std::shared_ptr<Input>& input) {
	if (input->actions["WAKE_UP"]) {
		this->switchState(EntityState::WAKING);
	}

	Entity::handleInput(input);
}

void GuardianEntity::update(float dtime) {
	if (this->currentState == EntityState::WAKING) {
		if (this->animation->isDone()) {
			this->switchState(EntityState::IDLE);
		}
	}

	Entity::update(dtime);
}

void GuardianEntity::switchState(EntityState to) {
	if (to == EntityState::WAKING) {
		this->animation = this->wakingAnimation;
	}

	Entity::switchState(to);
}