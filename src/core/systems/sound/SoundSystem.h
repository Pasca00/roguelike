#pragma once

#include "../ISystem.h"

#include <SDL_mixer.h>

#include <unordered_map>

class SoundSystem : public ISystem {
private:
	std::unordered_map<std::string, Mix_Chunk*> sounds;
	std::unordered_map<std::string, Mix_Music*> music;

public:
	SoundSystem();
	~SoundSystem();

	void init() override;

	void loadSound(std::string filePath, std::string name);
	void loadMusic(std::string filePath, std::string name);
	void deleteSound(std::string name);
	void deleteMusic(std::string name);

	void playMusic(std::string name);
	void playSound(std::string name, int loops = 0);

	void stopMusic();
};