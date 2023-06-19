#include "Item.h"

Item::Item(std::shared_ptr<Texture>& scrollTexture, std::string name, std::string description, float x, float y)
	: name(name), description(description) {

	this->scrollView = std::make_shared<View>(scrollTexture, x, y, 4);

	this->enabled = false;

	// TODO
	this->nameView = std::make_shared<TextView>(name, 64, 128, false, 0.5f);
	this->descriptionView = std::make_shared<TextView>(description, 64, 64, false, 0.3f);

	this->timeSinceEnabled = 0.f;
}

void Item::update(float dtime) {
	if (this->enabled) {
		this->timeSinceEnabled += dtime;
	};

	int direction = rand() % 3 - 1;
	this->scrollView->setY(this->scrollView->getY() + 8.f * direction * dtime);
}

void Item::enable() {
	this->enabled = true;
}

void Item::disable() {
	this->enabled = false;
}

bool Item::isEnabled() {
	return this->enabled;
}

bool Item::canPickup() {
	return this->enabled && this->timeSinceEnabled >= 0.5f;
}

std::shared_ptr<View>& Item::getScrollView() {
	return this->scrollView;
}

std::shared_ptr<TextView>& Item::getNameView() {
	return this->nameView;
}

std::shared_ptr<TextView>& Item::getDescriptionView() {
	return this->descriptionView;
}