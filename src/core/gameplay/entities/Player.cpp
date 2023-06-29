#include "Player.h"

Player::Player(
	std::shared_ptr<Movable>& movableComponent,
	std::vector<std::shared_ptr<AnimationView>>& idleAnimations,
	std::vector<std::shared_ptr<AnimationView>>& attackAnimations,
	std::vector<std::shared_ptr<AnimationView>>& moveAnimations,
	std::shared_ptr<AnimationView>& deadAnimation,
	std::shared_ptr<ControllableParameters>& controllableParams
) : Entity(movableComponent, idleAnimations, attackAnimations, moveAnimations, deadAnimation, 4) {
	this->controllableParams = controllableParams;

	this->energyResource = 100.f;
}

void Player::handleInput(std::shared_ptr<Input>& input) {
	if (input->actions["SHIFT"]) {
		this->movableComponent->dash();
	}

	if (input->actions["USE_ITEM"] && this->currItem != nullptr && !this->currItem->inUse && this->energyResource >= 100.f) {
		this->currItem->inUse = true;
		this->currItem->onEffectStart(this);
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

	if (this->currItem != nullptr) {
		if (!this->currItem->inUse) {
			if (this->energyResource < 100) {
				this->energyResource += 5 * dtime;
			}
		}
		else {
			if (this->energyResource <= 0) {
				this->currItem->inUse = false;
				this->currItem->onEffectEnd(this);
			}
			
			this->energyResource -= this->currItem->energyCost * dtime;
		}
	}

	switch (this->currentState) {
	case EntityState::IDLE:
		if (this->animation->isDone()) {
			this->animation = this->idleAnimations[rand() % idleAnimations.size()];
			this->animation->reset();
		}
		break;

	case EntityState::ATTACK:
		uint8_t currAttackFrame = this->animation->getCurrentFrame();
		if (currAttackFrame == this->attackFrame || currAttackFrame == this->attackFrame + 1) {
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
	m->combatableComponent->currHealth -= this->movableComponent->combatableComponent->getDamageDealt();
	m->combatableComponent->onDamageTaken();

	for (auto effectName : this->movableComponent->onHitApplies) {
		m->addStatusEffect(effectName);
	}
}

void Player::increaseAttackSpeed(float percent) {
	for (auto anim : this->attackAnimations) {
		anim->setFrameTime(anim->getFrameTime() * (1 - percent));
	}
}

void Player::addUsableItem(std::shared_ptr<UsableItem>& i) {
	this->currItem = i;
}