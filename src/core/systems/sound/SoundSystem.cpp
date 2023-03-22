#include "SoundSystem.h"

SoundSystem::SoundSystem() { }

SoundSystem::~SoundSystem() {
    for (const auto& sound : this->sounds) {
        Mix_FreeChunk(sound.second);
    }

    for (const auto& music : this->music) {
        Mix_FreeMusic(music.second);
    }

    Mix_Quit();
}

void SoundSystem::init() {
    printf("INITIALIZING SOUND SYSTEM\n");
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("ERROR::SDL_Mixer::Init:: %s\n", Mix_GetError());
    }
}

void SoundSystem::loadSound(std::string filePath, std::string name) {
    auto sound = Mix_LoadWAV(filePath.c_str());
    if (sound == NULL) {
        printf("ERROR::SOUND_SYSTEM::LOADING_CHUNK:: %s", name.c_str());
        return;
    }

    this->sounds[name] = sound;
}

void SoundSystem::loadMusic(std::string filePath, std::string name) {
    auto music = Mix_LoadMUS(filePath.c_str());
    if (music == NULL) {
        printf("ERROR::SOUND_SYSTEM::LOADING_CHUNK:: %s", name.c_str());
        return;
    }

    this->music[name] = music;
}

void SoundSystem::deleteSound(std::string name) {
    Mix_FreeChunk(this->sounds[name]);
    this->sounds.erase(name);
}

void SoundSystem::deleteMusic(std::string name) {
    Mix_FreeMusic(this->music[name]);
    this->music.erase(name);
}

void SoundSystem::playMusic(std::string name) {
    Mix_PlayMusic(this->music[name], -1);
}

void SoundSystem::playSound(std::string name, int loops) {
    Mix_PlayChannel(-1, this->sounds[name], loops);
}

void SoundSystem::stopMusic() {
    Mix_PauseMusic();
}

