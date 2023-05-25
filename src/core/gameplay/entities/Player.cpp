#include "Player.h"

Player::Player(
	std::shared_ptr<Movable>& movableComponent,
	std::vector<std::shared_ptr<AnimationView>>& idleAnimations,
	std::vector<std::shared_ptr<AnimationView>>& attackAnimations,
	std::vector<std::shared_ptr<AnimationView>>& moveAnimations,
	std::shared_ptr<AnimationView>& deadAnimation
) : Entity(movableComponent, idleAnimations, attackAnimations, moveAnimations, deadAnimation) {

}

void Player::handleInput(std::shared_ptr<Input>& input) {
	switch (this->currentState) {
	case EntityState::IDLE:
		if (input->actions["CLICK"]) {
			this->currentState = EntityState::ATTACK;
			this->animation = attackAnimations[rand() % attackAnimations.size()];
			this->animation->reset();
		} else {
			if (!input->actions["LEFT"] != !input->actions["RIGHT"]) {
				this->currentState = EntityState::MOVE;
				this->movableComponent->startMovement();
				this->animation = moveAnimations[rand() % moveAnimations.size()];
				this->animation->reset();
			}
		}

		break;

	case EntityState::MOVE:
		if (input->actions["LEFT"]) {
			this->movableComponent->setXDirection(-1);
		} else if (input->actions["RIGHT"]) {
			this->movableComponent->setXDirection(1);
		} else if (input->actions["DOWN"]) {
			this->movableComponent->setYDirection(-1);
		} else if (input->actions["UP"]) {
			this->movableComponent->setYDirection(1);
		} else {
			this->movableComponent->stopMovement();
			this->currentState = EntityState::IDLE;
			this->animation = idleAnimations[rand() % idleAnimations.size()];
			this->animation->reset();
		}

		break;
	}
}

void Player::update(float dtime) {
	this->animation->update(dtime);

	switch (this->currentState) {
	case EntityState::IDLE:
		if (this->animation->isDone()) {
			this->animation = this->idleAnimations[rand() % idleAnimations.size()];
			this->animation->reset();
		}
		break;

	case EntityState::ATTACK:
		if (this->animation->isDone()) {
			this->currentState = EntityState::IDLE;
			this->animation = this->attackAnimations[rand() % attackAnimations.size()];
			this->animation->reset();
		}
		break;
	}
}

std::shared_ptr<AnimationView> Player::getCurrentTexture() {
	this->animation->setX(this->movableComponent->position.x);
	this->animation->setY(this->movableComponent->position.y);

	return this->animation;
}