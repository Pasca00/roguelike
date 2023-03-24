#pragma once

#include "IState.h"
#include "../gameplay/levels/LevelManager.h"

class PlayingState : public IState {
private:
	std::unique_ptr<LevelManager> levelManager;

public:
	PlayingState(
		bool& stateChange,
		std::shared_ptr<InputSystem>& inputSystem,
		std::shared_ptr<VideoSystem>& videoSystem,
		std::shared_ptr<PhysicsSystem>& physicsSystem,
		std::shared_ptr<SoundSystem>& soundSystem,
		std::shared_ptr<GeneralSystem>& generalSystem
	);

	~PlayingState();

	virtual void handleInput() override;
	virtual void update(float dTime) override;
	virtual void render() override;
};