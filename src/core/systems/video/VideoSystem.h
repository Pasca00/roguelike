#pragma once

#include <memory>
#include <unordered_map>

#define GLEW_STATIC
#include <gl/glew.h>
#include <SDL_opengl.h>
#include <gl/GLU.h>
#include <gl/GL.h>

#include <SDL.h>

#include "../ISystem.h"
#include "Window.h"
#include "Renderer.h"
#include "Shader.h"
#include "textures/TextureManager.h"
#include "../../Paths.h"
#include "Framebuffer.h"

class VideoSystem : public ISystem {
private:
	std::unique_ptr<Window> window;
	std::unique_ptr<Renderer> renderer;
	std::shared_ptr<TextureManager> textureManager;

	std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;

	std::unique_ptr<Framebuffer> framebuffer;

	GLint window_fbo;

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

	std::shared_ptr<TextureManager> getTextureManager();

	void draw(std::shared_ptr<IView> view, std::string shaderName = "base");
	void draw(std::shared_ptr<Texture> texture, std::string shaderName = "base");

	void swapWindow();

	int getWindowWidth();
	int getWindowHeight();

	void initFramebuffer();
	void bindFrameBuffer(bool clearBuffer = true);
	void unbindFramebuffer();
	void drawFrameBuffer(std::string shaderName = "base", unsigned int time = 0);
};