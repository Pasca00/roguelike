#pragma once

#include <memory>

#include "../../../gameplay/Hitbox.h"
#include "../textures/Texture.h"

class View {
private:
	std::unique_ptr<Texture> texture;
	std::unique_ptr<Hitbox> hitbox;
	float size = 10;

public:
	View(std::unique_ptr<Texture>& texture, float x = 0, float y = 0, float size = 10);
	View(std::unique_ptr<Texture>& texture, std::unique_ptr<Hitbox>& hitbox, float size = 10);

	GLuint getTextureId();

	float getWidth();
	float getHeight();
	float getX();
	float getY();
};