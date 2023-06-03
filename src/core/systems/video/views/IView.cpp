#include "IView.h"

IView::IView(float x, float y, float w, float h, float size) {
	this->hitbox = std::make_unique<Hitbox>(x, y, w, h);
	this->size = size;
	this->renderFlipped = false;
}

IView::IView(std::unique_ptr<Hitbox>& hitbox, float size) {
	this->hitbox = std::move(hitbox);
	this->size = size;
	this->renderFlipped = false;
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
