#include "VideoSystem.h"
#include "../../../glm/glm.hpp"

#include <iostream>

void VideoSystem::init() {
	std::cout << "Initializing video system\n";

	this->initSDL();
	this->setGLAttributes();
	this->initWindow();
	this->initGL();
	this->initComponents();
	this->initShaders();
	this->loadInitialTextures();
	this->initUniforms();
}

void VideoSystem::initSDL() {
	SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_EVENTS);
}

void VideoSystem::initWindow() {
	this->window = std::make_unique<Window>(
		std::make_unique<WindowConfig>()
	);
}

void VideoSystem::initGL() {
	GLenum error = GL_NO_ERROR;

	glewExperimental = GL_TRUE;
	auto context = SDL_GL_CreateContext(window->getSDLWindow());
	if (context == NULL) {
		std::cout << "Failed to create OpenGL context\n";
	}

	this->window_fbo = 0;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &this->window_fbo);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	error = glewInit();
	if (error != GLEW_OK) {
		std::cout << "ERROR ON GLEW INIT " << glewGetErrorString(error) << std::endl;
	}

	glViewport(0, 0, this->window->getWidth(), this->window->getHeight());
}

void VideoSystem::initComponents() {
	this->textureManager = std::make_unique<TextureManager>();
	this->renderer = std::make_unique<Renderer>(this->window->getHeight(), this->window->getWidth());
}


void VideoSystem::setGLAttributes() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	//SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
}

void VideoSystem::initShaders() {
	this->shaders["base"] = std::make_shared<Shader>(Paths::SHADERS_DIR.c_str(), "Base");
	this->shaders["rain"] = std::make_shared<Shader>(Paths::SHADERS_DIR.c_str(), "Rain");
}

void VideoSystem::loadInitialTextures() {
	
}

void VideoSystem::initUniforms() {
	this->uintUniforms = std::unordered_map<std::string, unsigned int>();
	this->intUniforms = std::unordered_map<std::string, int>();
	this->floatUniforms = std::unordered_map<std::string, float>();
}

void VideoSystem::clearUniforms() {
	this->uintUniforms.clear();
	this->intUniforms.clear();
	this->floatUniforms.clear();
}

void VideoSystem::clearScreen() {
	glClearColor(0.5f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void VideoSystem::draw(std::shared_ptr<IView> view, std::string shaderName) {
	this->renderer->draw(
		view, 
		this->shaders[shaderName],
		this->uintUniforms,
		this->intUniforms,
		this->floatUniforms
	);

	this->clearUniforms();
}

void VideoSystem::draw(std::shared_ptr<Texture> texture, std::string shaderName) {
	this->renderer->draw(
		texture, 
		this->shaders[shaderName],
		this->uintUniforms,
		this->intUniforms,
		this->floatUniforms
	);

	this->clearUniforms();
}

std::shared_ptr<TextureManager> VideoSystem::getTextureManager() {
	return textureManager;
}

void VideoSystem::swapWindow() {
	SDL_GL_SwapWindow(this->window->getSDLWindow());	
}

int VideoSystem::getWindowWidth() {
	return this->window->getWidth();
}

int VideoSystem::getWindowHeight() {
	return this->window->getHeight();
}

void VideoSystem::initFramebuffer() {
	this->framebuffer = std::make_unique<Framebuffer>(
		this->window_fbo,
		this->textureManager, 
		getWindowWidth(),
		getWindowHeight(),
		4
	);
}

void VideoSystem::bindFrameBuffer(bool clearBuffer) {
	this->framebuffer->bind(clearBuffer);
}

void VideoSystem::unbindFramebuffer() {
	this->framebuffer->unbind();
	this->clearScreen();
}

void VideoSystem::drawFrameBuffer(std::string shaderName) {
	this->draw(this->framebuffer->getView(), shaderName);
}

void VideoSystem::setUintUniform(std::string key, unsigned int value) {
	this->uintUniforms[key] = value;
}

void VideoSystem::setIntUniform(std::string key, int value) {
	this->intUniforms[key] = value;
}

void VideoSystem::setFloatUniform(std::string key, float value) {
	this->floatUniforms[key] = value;
}