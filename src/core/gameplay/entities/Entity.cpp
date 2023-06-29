#include "Entity.h"
//#include "../inventory/GlobalEffectManager.h"

namespace colors {
	glm::vec4 red_hit		= glm::vec4(0.8f, 0.2f, 0.2f, 0.5f);
	glm::vec4 fire_burn		= glm::vec4(0.906f, 0.38f, 0.11f, 0.5f);
	glm::vec4 ice_slow		= glm::vec4(0.187f, 0.55f, 0.93f, 0.5f);
	glm::vec4 time_freeze	= glm::vec4(0.2f, 0.91f, 0.44f, 0.75f);
	glm::vec4 feared		= glm::vec4(0.39f, 0.2f, 0.92f, 0.75f);
	glm::vec4 none			= glm::vec4(0);
};

Entity::Entity(
	std::shared_ptr<Movable>& movableComponent,
	std::vector<std::shared_ptr<AnimationView>>& idleAnimations,
	std::vector<std::shared_ptr<AnimationView>>& attackAnimations,
	std::vector<std::shared_ptr<AnimationView>>& moveAnimations,
	std::shared_ptr<AnimationView>& deadAnimation,
	int attackFrame
) : movableComponent(movableComponent),
	animation(idleAnimations[0]),
	idleAnimations(idleAnimations),
	attackAnimations(attackAnimations),
	moveAnimations(moveAnimations),
	deadAnimation(deadAnimation) {

	this->currentState = EntityState::IDLE;
	this->drawFlipped = false;
	this->attackFrame = attackFrame;
	this->enabled = true;

}

void Entity::handleInput(std::shared_ptr<Input>& input) {
	if (!this->isEnabled()) {
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
	this->animation->update(dtime * this->movableComponent->timeModifier);
	if (this->currentState == EntityState::DEAD) {
		return;
	}

	for (auto& e = this->movableComponent->activeStatusEffects.begin(); e != this->movableComponent->activeStatusEffects.end();) {
		(*e).second->apply(dtime, this->movableComponent);
		if ((*e).second->isDone()) {
			e = this->movableComponent->activeStatusEffects.erase(e);
		} else {
			e++;
		}
	}

	/*for (auto& e : this->movableComponent->activeStatusEffects) {
		e.second->apply(dtime, this->movableComponent);
		if (e.second->isDone()) {
			this->movableComponent->activeStatusEffects.erase(e.first);
		}
	}*/

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
		if (currAttackFrame == this->attackFrame) {
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

	if (this->movableComponent->activeStatusEffects.count(EffectName::TIME_RIFT)) {
		this->animation->setOverlayColor(colors::time_freeze);
	}
	else if (this->movableComponent->combatableComponent->recentlyDamaged) {
		this->animation->setOverlayColor(colors::red_hit);
	}
	else if (this->movableComponent->activeStatusEffects.count(EffectName::BURN)) {
		this->animation->setOverlayColor(colors::fire_burn);
	}
	else if (this->movableComponent->activeStatusEffects.count(EffectName::SLOW)) {
		this->animation->setOverlayColor(colors::ice_slow);
	}
	else if (this->movableComponent->activeStatusEffects.count(EffectName::FEAR)) {
		this->animation->setOverlayColor(colors::feared);
	}
	else {
		this->animation->setOverlayColor(colors::none);
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
	}
	else if (to == EntityState::DEAD) {
		this->movableComponent->combatableComponent->onDeath();
	}

	this->currentState = to;
}

std::shared_ptr<Movable>& Entity::getMovableComponent() {
	return this->movableComponent;
}

void Entity::interactWithEnemy(std::shared_ptr<Movable>& m) {
	m->combatableComponent->onDamageTaken();
	m->combatableComponent->currHealth -= this->movableComponent->combatableComponent->attackDamage;
}

void Entity::enable() {
	this->movableComponent->disabled = false;
}

void Entity::disable() {
	this->movableComponent->disabled = true;
}

bool Entity::isEnabled() {
	return !this->movableComponent->disabled;
}

EntityState Entity::getCurrentState() {
	return this->currentState;
}