#include "IView.h"

IView::IView(float x, float y, float w, float h, float size) {
	this->hitbox = std::make_unique<Hitbox>(x, y, w, h);
	this->size = size;
	this->renderFlipped = false;

	this->overlayColor = glm::vec4(0);
}

IView::IView(std::unique_ptr<Hitbox>& hitbox, float size) {
	this->hitbox = std::move(hitbox);
	this->size = size;
	this->renderFlipped = false;

	this->overlayColor = glm::vec4(0);
}

IView::~IView() {

}

float IView::getWidth() {
	return this->hitbox->w;
}

float IView::getHeight() {
	return this->hitbox->h;
}

float IView::getX() {
	return this->hitbox->x;
}

float IView::getY() {
	return this->hitbox->y;
}

void IView::setX(float x) {
	this->hitbox->x = x;
}

void IView::setY(float y) {
	this->hitbox->y = y;
}

void IView::setWidth(float w) {
	this->hitbox->w = w;
}

void IView::setHeight(float h) {
	this->hitbox->h = h;
}

float IView::getSize() {
	return this->size;
}

void IView::update(float dTime) { }

void IView::flip(bool f) {
	this->renderFlipped = f;
}

bool IView::isFlipped() {
	return this->renderFlipped;
}

void IView::setOverlayColor(glm::vec4& color) {
	this->overlayColor = color;
}

glm::vec4& IView::getOverlayColor() {
	return this->overlayColor;
}