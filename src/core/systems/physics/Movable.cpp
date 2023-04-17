#include "Movable.h"

Movable::Movable(glm::vec2& position) : position(position) {
	this->direction = glm::vec2(1, 0);
	this->velocity = glm::vec2(0, 0);
}