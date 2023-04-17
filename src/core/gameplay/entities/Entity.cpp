#include "Entity.h"

Entity::Entity(std::shared_ptr<AnimationView>& animation) : animation(animation) { }

std::shared_ptr<AnimationView> Entity::getCurrentTexture() {
	return this->animation;
}