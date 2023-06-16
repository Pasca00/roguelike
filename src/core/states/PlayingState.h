#pragma once

#include "IState.h"
#include "../gameplay/levels/LevelManager.h"
#include "../gameplay/entities/Player.h"
#include "../gameplay/entities/enemies/AI/IController.h"

class PlayingState : public IState {
private:
	std::unique_ptr<LevelManager> levelManager;
	std::unique_ptr<Player> player;

	std::vector<std::unique_ptr<IController>> enemies;

	void makeEnemies();

	std::unique_ptr<Barrier> updateBarrier;

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

	virtual void executePostLoad() override;
};