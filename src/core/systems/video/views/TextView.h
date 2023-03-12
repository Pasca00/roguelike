#pragma once

#include <string>

#include "IView.h"

class TextView : public IView {
private:
	std::string text;

public:
	TextView(std::string text, float x = 0, float y = 0, float w = 0, float h = 0, float size = 1);
	TextView(std::string text, std::unique_ptr<Hitbox>& hitbox, float size = 1);

	std::string& getText();

	GLuint getTextureId() override;
};