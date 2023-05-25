//#pragma once
//
//#include "Entity.h"
//
//class PlayerIdleState : public Entity {
//private:
//	Uint8 w_animation1;
//	Uint8 w_animation2;
//
//	std::shared_ptr<AnimationView> animation1;
//	std::shared_ptr<AnimationView> animation2;
//
//public:
//	PlayerIdleState(
//		std::shared_ptr<AnimationView>& animation1,
//		std::shared_ptr<AnimationView>& animation2
//	);
//
//	void handleInput(std::shared_ptr<Input>& input) override;
//	void update(float dtime) override;
//};
//
//class PlayerWalkingState : public Entity {
//public:
//	PlayerWalkingState(std::shared_ptr<AnimationView>& animation);
//
//	void handleInput(std::shared_ptr<Input>& input) override;
//	void update(float dtime) override;
//};
//
//class PlayerAttackState : public Entity {
//public:
//	PlayerAttackState(std::shared_ptr<AnimationView>& animation);
//
//	void handleInput(std::shared_ptr<Input>& input) override;
//	void update(float dtime) override;
//};
//
//class PlayerDyingState : public Entity {
//public:
//	PlayerDyingState(std::shared_ptr<AnimationView>& animation);
//
//	void handleInput(std::shared_ptr<Input>& input) override;
//	void update(float dtime) override;
//};