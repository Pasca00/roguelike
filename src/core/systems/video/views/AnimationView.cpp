#include "AnimationView.h"

AnimationView::AnimationView(
	std::vector<std::unique_ptr<Texture>>& frames,
	float frameTime,
	float x,
	float y,
	float size
) : IView(x, y, frames[0]->getWidth(), frames[0]->getHeight(), size) {
	this->frames = frames;
	this->frameTime = frameTime;
}

AnimationView::AnimationView(
	std::vector<std::unique_ptr<Texture>>& frames,
	std::unique_ptr<Hitbox>& hitbox,
	float frameTime = 60,
	float size = 1
) : IView(hitbox, size) {
	this->frames = frames;
	this->frameTime = frameTime;
}

GLuint AnimationView::getTextureId() {
	return this->frames[this->current_frame]->getTextureId();
}

