#pragma once

#include "../systems/input/InputSystem.h"
#include "../systems/video/VideoSystem.h"
#include "../systems/physics/PhysicsSystem.h"
#include "../systems/sound/SoundSystem.h"
#include "../systems/general/GeneralSystem.h"

class IState {
protected:
	std::shared_ptr<InputSystem>& inputSystem;
	std::shared_ptr<VideoSystem>& videoSystem;
	std::shared_ptr<SoundSystem>& soundSystem;
	std::shared_ptr<PhysicsSystem>& physicsSystem;
	std::shared_ptr<GeneralSystem>& generalSystem;

	bool& stateChange;

public:
	IState(
		bool& stateChange,
		std::shared_ptr<InputSystem>& inputSystem,
		std::shared_ptr<VideoSystem>& videoSystem,
		std::shared_ptr<PhysicsSystem>& physicsSystem,
		std::shared_ptr<SoundSystem>& soundSystem,
		std::shared_ptr<GeneralSystem>& generalSystem
	) : stateChange(stateChange),
		inputSystem(inputSystem), 
		videoSystem(videoSystem), 
		physicsSystem(physicsSystem),
		soundSystem(soundSystem),
		generalSystem(generalSystem) { }

	virtual ~IState() {}

	virtual void handleInput() = 0;
	virtual void update(float dTime) = 0;
	virtual void render() = 0;
};