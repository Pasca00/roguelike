#pragma once

#include <string>

#include "IView.h"

class TextView : public IView {
private:
	std::string text;
	bool clickable;

public:
	TextView(std::string text, float x = 0, float y = 0, bool clickable = false, float size = 1);
	TextView(std::string text, std::unique_ptr<Hitbox>& hitbox, float size = 1);

	std::string& getText();

	GLuint getTextureId() override;
};