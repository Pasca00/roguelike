#pragma once

#include <memory>

#include "../../gameplay/Hitbox.h"

class Camera {
private:
	std::shared_ptr<Hitbox> subject;

	std::shared_ptr<Hitbox> focusZone;
	std::shared_ptr<Hitbox> viewArea;

public:
	Camera(int windowWidth, int windowHeight);

	void setSubject(std::shared_ptr<Hitbox>& subject);
	void update(float dTime);
};