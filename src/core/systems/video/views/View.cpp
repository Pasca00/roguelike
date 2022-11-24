#include "View.h"

View::View(
	std::unique_ptr<Texture>& texture, 
	float x, 
	float y, 
	float size
) : IView(x, y, texture->getWidth(), texture->getHeight()) {
	this->texture = std::move(texture);
}

View::View(
	std::unique_ptr<Texture>& texture,
	std::unique_ptr<Hitbox>& hitbox, 
	float size
) : IView(std::move(hitbox), size) {
	this->texture = std::move(texture);
}

GLuint View::getTextureId() {
	return this->texture->getTextureId();
}