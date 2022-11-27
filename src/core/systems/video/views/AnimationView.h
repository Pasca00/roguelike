#pragma once

#include "IView.h"

#include <vector>

class AnimationView : public IView {
public:
	std::vector<std::unique_ptr<Texture>> frames;
	uint8_t currentFrame;

	float frameTime;
	float timeSinceLastFrame;

private:
	AnimationView(
		std::vector<std::unique_ptr<Texture>>& frames,
		float frameTime = 60,
		float x = 0,
		float y = 0,
		float size = 1
	);
	AnimationView(
		std::vector<std::unique_ptr<Texture>>& frames,
		std::unique_ptr<Hitbox>& hitbox,
		float frameTime = 60,
		float size = 1
	);

	GLuint getTextureId();

	void update(float dTime);
};

