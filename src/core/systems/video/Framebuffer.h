#pragma once

#include <iostream>

#include <gl/glew.h>

#include "textures/TextureManager.h"
#include "views/View.h"

class Framebuffer {
private:
	unsigned int fbo;
	GLenum drawBuffer;

	std::shared_ptr<View> framebufferView;

	GLenum window_fbo;

	int width;
	int height;

public:
	Framebuffer(GLint window_fbo, std::shared_ptr<TextureManager>& textureManager, int width, int height, int channels);
	~Framebuffer();

	void bind(bool clearBuffer = true);
	void unbind();

	std::shared_ptr<View> getView();
};