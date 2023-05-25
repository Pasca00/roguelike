#include "Entity.h"

Entity::Entity(
	std::shared_ptr<Movable>& movableComponent,
	std::vector<std::shared_ptr<AnimationView>>& idleAnimations,
	std::vector<std::shared_ptr<AnimationView>>& attackAnimations,
	std::vector<std::shared_ptr<AnimationView>>& moveAnimations,
	std::shared_ptr<AnimationView>& deadAnimation
) : movableComponent(movableComponent),
	animation(idleAnimations[0]),
	idleAnimations(idleAnimations),
	attackAnimations(attackAnimations),
	moveAnimations(moveAnimations),
	deadAnimation(deadAnimation) {
	this->currentState = EntityState::IDLE;
}

std::shared_ptr<AnimationView> Entity::getCurrentTexture() {
	return this->animation;
}