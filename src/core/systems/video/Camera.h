#pragma once

#include <memory>

#include "../../../glm/glm.hpp"
#include "../../../glm/gtc/matrix_transform.hpp"
//#include "../../gameplay/Hitbox.h"
#include "../physics/Movable.h"

class Camera {
private:
	static const float FOCUS_SIZE_RATIO;

	std::shared_ptr<Hitbox> subject;

	//std::shared_ptr<Hitbox> focusZone;
	std::shared_ptr<Movable> focusZone;
	std::shared_ptr<Hitbox> viewArea;

	glm::mat4 viewMatrix;

	float focus_x_diff;
	float focus_y_diff;

public:
	Camera(int windowWidth, int windowHeight);

	void setSubject(std::shared_ptr<Hitbox>& subject);
	void update(float dTime);

	std::shared_ptr<Movable>& getFocusZone();

	float getX();
	float getY();

	glm::mat4& getViewMatrix();
};