#include "Camera.h"

Camera::Camera(int windowWidth, int windowHeight) {
	this->viewArea = std::make_shared<Hitbox>(0, 0, windowWidth, windowHeight);
}

void Camera::setSubject(std::shared_ptr<Hitbox>& subject) {
	this->subject = subject;
}

void Camera::update(float dTime) {

}