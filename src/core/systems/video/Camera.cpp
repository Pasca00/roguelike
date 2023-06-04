#include "Camera.h"

const float Camera::FOCUS_SIZE_RATIO = 0.15;

Camera::Camera(int windowWidth, int windowHeight) {
	float focusWidth = FOCUS_SIZE_RATIO * windowWidth;
	float focusHeight = FOCUS_SIZE_RATIO * windowHeight;

	auto hb = std::make_shared<Hitbox>(
		(windowWidth - focusWidth) / 2,
		(windowHeight - focusHeight) / 2,
		focusWidth, 
		focusHeight
	);
	this->focusZone = std::make_shared<Movable>(hb, 7.5f);
	this->focusZone->setCollision(false);

	this->viewArea = std::make_shared<Hitbox>(0, 0, windowWidth, windowHeight);

	this->focus_x_diff = hb->x;
	this->focus_y_diff = hb->y;
}

void Camera::setSubject(std::shared_ptr<Hitbox>& subject) {
	this->subject = subject;
}

void Camera::update(float dTime) {
	this->viewArea->x = this->focusZone->hitbox->x - this->focus_x_diff;
	this->viewArea->y = this->focusZone->hitbox->y - this->focus_y_diff;

	this->focusZone->stopMovement();

	auto& focus = this->focusZone->hitbox;

	if (this->subject->x < focus->x) {
		this->focusZone->setXDirection(-1);
		this->focusZone->startMovement();
	} else if (focus->x + focus->w < this->subject->x + this->subject->w) {
		this->focusZone->setXDirection(1);
		this->focusZone->startMovement();
	} else {
		this->focusZone->setXDirection(0);
	}

	if (this->subject->y < focus->y) {
		this->focusZone->setYDirection(-1);
		this->focusZone->startMovement();
	} else if (focus->y + focus->h < this->subject->y + this->subject->h) {
		this->focusZone->setYDirection(1);
		this->focusZone->startMovement();
	} else {
		this->focusZone->setYDirection(0);
	}
}

std::shared_ptr<Movable>& Camera::getFocusZone() {
	return this->focusZone;
}

float Camera::getX(){
	return this->viewArea->x;
}

float Camera::getY(){
	return this->viewArea->y;
}