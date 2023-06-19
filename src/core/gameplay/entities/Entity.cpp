#include "Entity.h"
//#include "../inventory/GlobalEffectManager.h"

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
	this->drawFlipped = false;
}

void Entity::handleInput(std::shared_ptr<Input>& input) {
	if (!this->enabled) {
		return;
	}

	switch (this->currentState) {
	case EntityState::IDLE:
		if (input->actions["CLICK"]) {
			this->switchState(EntityState::ATTACK);
		}
		else {
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

void Entity::update(float dtime) {
	/*for (auto& e : this->movableComponent->combatableComponent->activeStatusEffects) {
		e->apply(dtime, shared_from_this());
	}*/

	this->animation->update(dtime);
	if (this->currentState == EntityState::DEAD) {
		return;
	}

	if (this->currentState == EntityState::DYING) {
		if (this->animation->isDone()) {
			this->switchState(EntityState::DEAD);
		}
		return;
	}

	if (this->movableComponent->combatableComponent->currHealth <= 0) {
		this->switchState(EntityState::DYING);
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
		if (currAttackFrame == ENTITY_ATTACK_FRAME || currAttackFrame == ENTITY_ATTACK_FRAME + 1) {
			this->movableComponent->combatableComponent->isAttacking = true;
		} else {
			this->movableComponent->combatableComponent->isAttacking = false;
		}

		if (this->animation->isDone()) {
			this->currentState = EntityState::IDLE;
			this->animation = this->attackAnimations[rand() % attackAnimations.size()];
			this->animation->reset();
		}
		break;
	}
}

std::shared_ptr<AnimationView> Entity::getCurrentTexture() {
	this->animation->setX(this->movableComponent->hitbox->x - 8 * this->animation->getSize());
	this->animation->setY(this->movableComponent->hitbox->y);

	this->animation->flip(this->drawFlipped);
	if (this->movableComponent->combatableComponent->recentlyDamaged) {
		this->animation->setOverlayColor(glm::vec4(0.8, 0.2, 0.2, 0.5));
	} else {
		this->animation->setOverlayColor(glm::vec4(0));
	}

	return this->animation;
}

void Entity::switchState(EntityState to) {
	if (to == EntityState::MOVE) {
		this->movableComponent->startMovement();
		this->animation = moveAnimations[rand() % moveAnimations.size()];
		this->animation->reset();
	} else if (to == EntityState::IDLE) {
		this->movableComponent->stopMovement();
		this->movableComponent->setXDirection(0);
		this->movableComponent->setYDirection(0);
		this->animation = idleAnimations[rand() % idleAnimations.size()];
		this->animation->reset();
	} else if (to == EntityState::ATTACK) {
		this->movableComponent->combatableComponent->onAttack();
		this->movableComponent->startMovement();
		this->animation = attackAnimations[rand() % attackAnimations.size()];
		this->animation->reset();
	} else if (to == EntityState::DYING) {
		this->movableComponent->stopMovement();
		this->movableComponent->setXDirection(0);
		this->movableComponent->setYDirection(0);

		this->animation = deadAnimation;
		this->animation->reset();
	} else if (to == EntityState::DEAD) {
		this->movableComponent->combatableComponent->onDeath();
	}

	this->currentState = to;
}

std::shared_ptr<Movable>& Entity::getMovableComponent() {
	return this->movableComponent;
}

//void Entity::addOnHitEffect(EffectName effectName) {
//	this->onHitApplies.push_back(effectName);
//}
//
//void Entity::addStatusEffect(EffectName effectName) {
//	this->activeStatusEffects.push_back(GlobalEffectManager::makeEffect(effectName));
//}

void Entity::interactWithEnemy(std::shared_ptr<Movable>& m) {

}

void Entity::enable() {
	this->enabled = true;
}

void Entity::disable() {
	this->enabled = false;
}

bool Entity::isEnabled() {
	return this->enabled;
}