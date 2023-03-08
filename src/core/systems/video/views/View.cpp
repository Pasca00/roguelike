#include "View.h"

View::View(
	std::shared_ptr<Texture>& texture, 
	float x, 
	float y, 
	float size
) : IView(x, y, texture->getWidth(), texture->getHeight(), size) {
	this->texture = texture;
}

View::View(
	std::shared_ptr<Texture>& texture,
	std::unique_ptr<Hitbox>& hitbox, 
	float size
) : IView(std::move(hitbox), size) {
	this->texture = texture;
}

GLuint View::getTextureId() {
	return this->texture->getTextureId();
}