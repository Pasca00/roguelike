#include "View.h"

View::View(std::unique_ptr<Texture>& texture, float x, float y, float size) {
	this->texture = std::move(texture);
	this->hitbox = std::make_unique<Hitbox>(
		x,
		y,
		this->texture->getWidth(),
		this->texture->getHeight()
	);

	this->size = size;
}

View::View(std::unique_ptr<Texture>& texture, std::unique_ptr<Hitbox>& hitbox, float size) {
	this->texture = std::move(texture);
	this->hitbox = std::move(hitbox);
	this->size = size;
}

GLuint View::getTextureId() {
	return this->texture->getTextureId();
}

float View::getWidth() {
	return this->hitbox->w;
}

float View::getHeight() {
	return this->hitbox->h;
}

float View::getX() {
	return this->hitbox->x;
}

float View::getY() {
	return this->hitbox->y;
}