#pragma once

#include "../../../glm/glm.hpp"

class Movable {
protected:
	glm::vec2 velocity;
	glm::vec2 direction;
	glm::vec2 position;

public:
	Movable(glm::vec2& position);
};