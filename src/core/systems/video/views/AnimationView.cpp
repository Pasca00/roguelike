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
}

AnimationView::AnimationView(
	std::vector<std::unique_ptr<Texture>>& frames,
	std::unique_ptr<Hitbox>& hitbox,
	float frameTime,
	float size
) : IView(std::move(hitbox), size) {
	this->frames = std::move(frames);
	this->frameTime = frameTime;
}

GLuint AnimationView::getTextureId() {
	return this->frames[this->current_frame]->getTextureId();
}

