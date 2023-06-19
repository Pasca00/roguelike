#include "AnimationView.h"

AnimationView::AnimationView(
	std::vector<std::shared_ptr<Texture>>& frames,
	bool repeat,
	float frameTime,
	float x,
	float y,
	float size
) : IView(
	x, 
	y, 
	static_cast<float>(frames[0]->getWidth()),
	static_cast<float>(frames[0]->getHeight()),
	size
) {
	this->frames = frames;
	this->frameTime = frameTime;
	this->currentFrame = 0;
	this->timeSinceLastFrame = 0;
	this->repeat = repeat;
}

AnimationView::AnimationView(
	std::vector<std::shared_ptr<Texture>>& frames,
	std::unique_ptr<Hitbox>& hitbox,
	bool repeat,
	float frameTime,
	float size
) : IView(std::move(hitbox), size) {
	this->frames = frames;
	this->frameTime = frameTime;
	this->currentFrame = 0;
	this->timeSinceLastFrame = 0;
	this->repeat = repeat;
}

GLuint AnimationView::getTextureId() {
	return this->frames[this->currentFrame]->getTextureId();
}

uint8_t AnimationView::getCurrentFrame() {
	return this->currentFrame;
}

void AnimationView::update(float dTime) {
	if (!this->repeat && this->isDone()) {
		return;
	}

	this->timeSinceLastFrame += dTime;
	if (this->timeSinceLastFrame >= this->frameTime) {
		this->currentFrame++;
		
		if (this->repeat) {
			this->currentFrame %= this->frames.size();
		}

		this->timeSinceLastFrame = 0;
	}
}

bool AnimationView::isDone() {
	return this->currentFrame == this->frames.size() - 1;
}

void AnimationView::reset() {
	this->currentFrame = 0;
	this->timeSinceLastFrame = 0;
}

