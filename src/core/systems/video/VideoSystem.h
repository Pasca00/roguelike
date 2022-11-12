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
#include "textures/TextureManager.h"

class VideoSystem : public ISystem {
private:
	std::unique_ptr<Window> window;
	std::unique_ptr<TextureManager> textureManager;

	void initSDL();
	void setGLAttributes();
	void initWindow();
	void initGL();
	void initComponents();
	void initShaders();
	void loadInitialTextures();

public:
	void init();

	void clearScreen();
};