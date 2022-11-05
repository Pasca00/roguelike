#pragma once

#include <memory>

#define GLEW_STATIC
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <gl/GL.h>

#include <SDL.h>

#include "../ISystem.h"
#include "Window.h"

class VideoSystem : public ISystem {
private:
	std::unique_ptr<Window> window;

	void initSDL();
	void setGLAttributes();
	void initWindow();
	void initGL();
	void initShaders();
	void loadInitialTextures();

public:
	void init();

	void clearScreen();
};