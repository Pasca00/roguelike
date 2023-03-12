#include "TextView.h"

TextView::TextView(std::string text, float x, float y, float w, float h, float size)
	: IView(x, y, w, h, size) {
	this->text = text;
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