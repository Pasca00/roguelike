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

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
}

void VideoSystem::initShaders() {
	this->shaders["base"] = std::make_shared<Shader>(Paths::SHADERS_DIR.c_str(), "Base");
}

void VideoSystem::loadInitialTextures() {
	
}
	
void VideoSystem::clearScreen() {
	glClearColor(0.5f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void VideoSystem::draw(std::shared_ptr<IView> view, std::string shaderName) {
	this->renderer->draw(view, this->shaders[shaderName]);
}

void VideoSystem::draw(std::shared_ptr<Texture> texture, std::string shaderName) {
	this->renderer->draw(texture, this->shaders[shaderName]);
}

std::shared_ptr<TextureManager> VideoSystem::getTextureManager() {
	return textureManager;
}

void VideoSystem::swapWindow() {
	SDL_GL_SwapWindow(this->window->getSDLWindow());
}
