#include "Player.h"

Player::Player(
	std::shared_ptr<Movable>& movableComponent,
	std::vector<std::shared_ptr<AnimationView>>& idleAnimations,
	std::vector<std::shared_ptr<AnimationView>>& attackAnimations,
	std::vector<std::shared_ptr<AnimationView>>& moveAnimations,
	std::shared_ptr<AnimationView>& deadAnimation,
	std::shared_ptr<ControllableParameters>& controllableParams
) : Entity(movableComponent, idleAnimations, attackAnimations, moveAnimations, deadAnimation) {
	this->controllableParams = controllableParams;
}

void Player::handleInput(std::shared_ptr<Input>& input) {
	if (input->actions["SHIFT"]) {
		this->movableComponent->dash();
	}

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
		if (input->actions["CLICK"]) {
			this->switchState(EntityState::ATTACK);
		}

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
	
	
	case EntityState::ATTACK:
		if (input->actions["LEFT"]) {
			this->movableComponent->setXDirection(-1);
			this->drawFlipped = true;
		}
		else if (input->actions["RIGHT"]) {
			this->movableComponent->setXDirection(1);
			this->drawFlipped = false;
		}
		else {
			this->movableComponent->setXDirection(0);
		}

		if (input->actions["DOWN"]) {
			this->movableComponent->setYDirection(-1);
		}
		else if (input->actions["UP"]) {
			this->movableComponent->setYDirection(1);
		}
		else {
			this->movableComponent->setYDirection(0);
		}
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
		uint8_t currAttackFrame = this->animation->getCurrentFrame();
		if (currAttackFrame == ENTITY_ATTACK_FRAME || currAttackFrame == ENTITY_ATTACK_FRAME + 1) {
			this->movableComponent->combatableComponent->isAttacking = true;
		} else {
			this->movableComponent->combatableComponent->isAttacking = false;
		}

		if (this->animation->isDone()) {
			this->currentState = EntityState::IDLE;
			this->animation = this->idleAnimations[rand() % idleAnimations.size()];
			this->animation->reset();
		}
		break;
	}
}

void Player::interactWithEnemy(std::shared_ptr<Movable>& m) {
	m->combatableComponent->onDamageTaken();
	m->combatableComponent->currHealth -= this->movableComponent->combatableComponent->attackDamage;

	for (auto effectName : this->movableComponent->onHitApplies) {
		printf("%d\n", effectName);
		m->addStatusEffect(effectName);
	}
}