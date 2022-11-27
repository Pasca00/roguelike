#include "AnimationView.h"

AnimationView::AnimationView(
	std::vector<std::unique_ptr<Texture>>& frames,
	float frameTime,
	float x,
	float y,
	float size
) : IView(x, y, frames[0]->getWidth(), frames[0]->getHeight(), size) {
	this->frames = std::move(frames);
	this->frameTime = frameTime;
	this->currentFrame = 0;
	this->timeSinceLastFrame = 0;
}

AnimationView::AnimationView(
	std::vector<std::unique_ptr<Texture>>& frames,
	std::unique_ptr<Hitbox>& hitbox,
	float frameTime,
	float size
) : IView(std::move(hitbox), size) {
	this->frames = std::move(frames);
	this->frameTime = frameTime;
	this->currentFrame = 0;
	this->timeSinceLastFrame = 0;
}

GLuint AnimationView::getTextureId() {
	return this->frames[this->currentFrame]->getTextureId();
}

void AnimationView::update(float dTime) {
	this->timeSinceLastFrame += dTime;
	if (this->timeSinceLastFrame >= this->frameTime) {
		this->currentFrame++;
		this->currentFrame %= this->frames.size();
		this->timeSinceLastFrame = 0;
	}
}

