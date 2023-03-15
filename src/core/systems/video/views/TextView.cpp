#include "TextView.h"

TextView::TextView(std::string text, float x, float y, bool clickable, float size)
	: IView(x, y, 0, 0, size) {
	this->text = text;
	this->clickable = clickable;
}

TextView::TextView(std::string text, std::unique_ptr<Hitbox>& hitbox, float size)
	: IView(hitbox, size) {
	this->text = text;
}

std::string& TextView::getText() {
	return this->text;
}

GLuint TextView::getTextureId() {
	return -1;
}