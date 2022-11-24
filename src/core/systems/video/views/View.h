#pragma once

#include "IView.h"

class View : public IView {
private:
	std::unique_ptr<Texture> texture;

public:
	View(std::unique_ptr<Texture>& texture, float x = 0, float y = 0, float size = 1);
	View(std::unique_ptr<Texture>& texture, std::unique_ptr<Hitbox>& hitbox, float size = 1);

	GLuint getTextureId() override;
};