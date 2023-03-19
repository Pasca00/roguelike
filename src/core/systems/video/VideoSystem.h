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

#define TRANSITION_NONE		 0
#define TRANSITION_EASE_IN   1
#define TRANSITION_HOLD		 2
#define TRANSITION_EASE_OUT  3

class VideoSystem : public ISystem {
private:
	std::unique_ptr<Window> window;
	std::unique_ptr<Renderer> renderer;
	std::shared_ptr<TextureManager> textureManager;

	std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
	
	Character* characters[128];

	std::unique_ptr<Framebuffer> framebuffer;

	std::unordered_map<std::string, unsigned int> uintUniforms;
	std::unordered_map<std::string, int> intUniforms;
	std::unordered_map<std::string, float> floatUniforms;

	GLint window_fbo;

	int8_t transition;
	float transitionTime;
	float transitionEaseTime;
	float transitionHoldTime;
	std::shared_ptr<Texture> tranistionTexture;

	void initSDL();
	void setGLAttributes();
	void initWindow();
	void initGL();
	void initComponents();
	void initShaders();
	void loadInitialTextures();
	void initUniforms();
	void initTransitionUtils();

	void clearUniforms();

	void loadGlyphs();

public:
	void init();

	void clearScreen();

	std::shared_ptr<TextureManager> getTextureManager();

	void draw(std::shared_ptr<IView> view, std::string shaderName = "base");
	void draw(std::shared_ptr<Texture> texture, std::string shaderName = "base");
	void drawText(std::shared_ptr<TextView> textView, std::string shaderName = "text");

	void swapWindow();

	int getWindowWidth();
	int getWindowHeight();

	void initFramebuffer();
	void bindFrameBuffer(bool clearBuffer = true);
	void unbindFramebuffer();
	void drawFrameBuffer(std::string shaderName = "base");

	void setUintUniform(std::string key, unsigned int value);
	void setIntUniform(std::string key, int value);
	void setFloatUniform(std::string key, float value);

	glm::ivec2 getTextDimensions(std::string text);
	glm::ivec2 getCenteredTextPosition(std::string& text, std::shared_ptr<Hitbox> rect = NULL);

	void beginTransition();
	void updateTransition(float dTime);
	void drawTransition();
	int8_t getTransition();
	/* TODO: HANDLE TRANSITION EFFECT (EASE IN, HOLD, EASE OUT) */
};