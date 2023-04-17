#include "Player.h"

Player::Player(
	glm::vec2& position,
	std::shared_ptr<AnimationView>& idleAnimation1,
	std::shared_ptr<AnimationView>& idleAnimation2,
	std::shared_ptr<AnimationView>& attackAnimation,
	std::shared_ptr<AnimationView>& walkingAnimation,
	std::shared_ptr<AnimationView>& deathAnimation
) {
	this->currentState = Player::STATE_IDLE;
	this->movable = std::make_shared<Movable>(position);

	this->idleState = std::make_unique<PlayerIdleState>(idleAnimation1, idleAnimation2);
	this->attackState = std::make_unique<PlayerAttackState>(attackAnimation);
	this->walkingState = std::make_unique<PlayerWalkingState>(walkingAnimation);
	this->dyingState = std::make_unique<PlayerDyingState>(deathAnimation);

	this->currentState = this->idleState.get();
}

void Player::handleInput(std::shared_ptr<Input>& input) {
	if (input->actions["LEFT"]) {
		this->currentState = this->walkingState.get();
	}
	else {
		this->currentState = this->idleState.get();
	}

	this->currentState->handleInput(input);
}

void Player::update(float dtime) {
	this->currentState->update(dtime);
}

std::shared_ptr<AnimationView> Player::getCurrentTexture() {
	return this->currentState->getCurrentTexture();
}