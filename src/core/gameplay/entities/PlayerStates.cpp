//#include "PlayerStates.h"
//
//PlayerIdleState::PlayerIdleState(
//	std::shared_ptr<AnimationView>& animation1,
//	std::shared_ptr<AnimationView>& animation2
//) : Entity(animation1), animation1(animation1), animation2(animation2) {
//	this->w_animation1 = 90;
//	this->w_animation2 = 10;
//}
//
//void PlayerIdleState::handleInput(std::shared_ptr<Input>& input) {
//
//}
//
//void PlayerIdleState::update(float dtime) {
//	this->animation->update(dtime);
//
//	if (this->animation->isDone()) {
//		int chance = rand() % 100;
//		if (chance < this->w_animation1) {
//			this->animation = this->animation1;
//		}
//		else {
//			this->animation = this->animation2;
//		}
//	
//		this->animation->reset();
//	}
//
//}
//
//PlayerWalkingState::PlayerWalkingState(std::shared_ptr<AnimationView>& animation) : Entity(animation) { }
//void PlayerWalkingState::handleInput(std::shared_ptr<Input>& input) { }
//void PlayerWalkingState::update(float dtime) { this->animation->update(dtime); }
//
//PlayerAttackState::PlayerAttackState(std::shared_ptr<AnimationView>& animation) : Entity(animation) { }
//void PlayerAttackState::handleInput(std::shared_ptr<Input>& input) { }
//void PlayerAttackState::update(float dtime) { }
//
//PlayerDyingState::PlayerDyingState(std::shared_ptr<AnimationView>& animation) : Entity(animation) { }
//void PlayerDyingState::handleInput(std::shared_ptr<Input>& input) { }
//void PlayerDyingState::update(float dtime) { }