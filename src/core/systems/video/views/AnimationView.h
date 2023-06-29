#pragma once

#include "IView.h"

#include <vector>

class AnimationView : public IView {
private:
	std::vector<std::shared_ptr<Texture>> frames;
	uint8_t currentFrame;

	float frameTime;
	float timeSinceLastFrame;

	bool repeat;

public:
	AnimationView(
		std::vector<std::shared_ptr<Texture>>& frames,
		bool repeat = true,
		float frameTime = 0.15f,
		float x = 0.f,
		float y = 0.f,
		float size = 10.f
	);
	AnimationView(
		std::vector<std::shared_ptr<Texture>>& frames,
		std::unique_ptr<Hitbox>& hitbox,
		bool repeat = true,
		float frameTime = 0.15f,
		float size = 10.f
	);

	GLuint getTextureId();

	uint8_t getCurrentFrame();

	void update(float dTime) override;

	float getFrameTime();
	void setFrameTime(float);

	bool isDone();
	void reset();
};

