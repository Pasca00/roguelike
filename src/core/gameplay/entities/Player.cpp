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
			this->switchState(EntityState::ATTACK);
		} else {
			if (
				input->actions["LEFT"] || input->actions["RIGHT"]
				|| input->actions["UP"] || input->actions["DOWN"]
			) {
				this->switchState(EntityState::MOVE);
			}
		}

		break;

	case EntityState::MOVE:
		if (input->actions["LEFT"]) {
			this->movableComponent->setXDirection(-1);
			this->drawFlipped = true;
		} else if (input->actions["RIGHT"]) {
			this->movableComponent->setXDirection(1);
			this->drawFlipped = false;
		} else {
			this->movableComponent->setXDirection(0);
		}
		
		if (input->actions["DOWN"]) {
			this->movableComponent->setYDirection(-1);
		} else if (input->actions["UP"]) {
			this->movableComponent->setYDirection(1);
		} else {
			this->movableComponent->setYDirection(0);
		}

		if (!input->actions["LEFT"] && !input->actions["RIGHT"]
			&& !input->actions["UP"] && !input->actions["DOWN"]) {
			this->switchState(EntityState::IDLE);
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
	this->animation->setX(this->movableComponent->hitbox->x - 8 * this->animation->getSize());
	this->animation->setY(this->movableComponent->hitbox->y);

	this->animation->flip(this->drawFlipped);

	return this->animation;
}