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

void Entity::switchState(EntityState to) {
	if (to == EntityState::MOVE) {
		this->movableComponent->startMovement();
		this->animation = moveAnimations[rand() % moveAnimations.size()];
	} else if (to == EntityState::IDLE) {
		this->movableComponent->stopMovement();
		this->movableComponent->setXDirection(0);
		this->movableComponent->setYDirection(0);
		this->animation = idleAnimations[rand() % idleAnimations.size()];
	} else if (to == EntityState::ATTACK) {
		this->movableComponent->startMovement();
		this->animation = deadAnimation;
	}

	this->currentState = to;
	this->animation->reset();
}