#pragma once

#include <memory>

#include "../../../gameplay/Hitbox.h"
#include "../textures/Texture.h"

class IView {
private:
	std::unique_ptr<Hitbox> hitbox;

	float size;

public:
	IView(float x = 0, float y = 0, float w = 0, float h = 0, float size = 1);
	IView(std::unique_ptr<Hitbox>& hitbox, float size = 1);

	float getWidth();
	float getHeight();
	float getX();
	float getY();

	float getSize();

	virtual GLuint getTextureId() = 0;
};
