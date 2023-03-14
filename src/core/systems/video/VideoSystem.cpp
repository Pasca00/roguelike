#include "VideoSystem.h"
#include "../../../glm/glm.hpp"

#include <iostream>

#include <ft2build.h>
#include <freetype/freetype.h>
#include <freetype/ftmm.h>
#include <freetype/ftglyph.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../../../stb/stb_image_write.h"

void VideoSystem::init() {
	std::cout << "Initializing video system\n";

	this->initSDL();
	this->setGLAttributes();
	this->initWindow();
	this->initGL();
	this->loadGlyphs();
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
	this->renderer = std::make_unique<Renderer>(this->window->getHeight(), this->window->getWidth(), this->characters);
}

void VideoSystem::setGLAttributes() {
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);

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
	this->shaders["text"] = std::make_shared<Shader>(Paths::SHADERS_DIR.c_str(), "Text");
}

void VideoSystem::loadInitialTextures() {
	
}

void VideoSystem::initUniforms() {
	this->uintUniforms = std::unordered_map<std::string, unsigned int>();
	this->intUniforms = std::unordered_map<std::string, int>();
	this->floatUniforms = std::unordered_map<std::string, float>();
}

void VideoSystem::loadGlyphs() {
	FT_Library library;
	auto error = FT_Init_FreeType(&library);
	if (error) {
		printf("ERROR OCCURED DURING FREETYPE INITIALIZATION\n");
	}

	FT_Face face;
	if (FT_New_Face(library, (Paths::FONTS_DIR + "Pixellari.ttf").c_str(), 0, &face)) {
		printf("ERROR FAILED TO LOAD FREETYPE FONT");
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	FT_Set_Pixel_Sizes(face, 0, 48);
	for (unsigned char c = 0; c < 128; c++) {
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR FAILED TO LOAD GLYPH: " << c << '\n';
			continue;
		}

		GLuint textureId;
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_R8,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// now store character for later use
		Character* character = new Character(
			textureId,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		);
		this->characters[c] = character;
	}


	FT_Done_Face(face);
	FT_Done_FreeType(library);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
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

void VideoSystem::drawText(std::shared_ptr<TextView> textView, std::string shaderName) {
	this->renderer->drawText(
		textView, 
		this->shaders[shaderName],
		this->uintUniforms,
		this->intUniforms,
		this->floatUniforms
	);
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

glm::ivec2 VideoSystem::getTextDimensions(std::string text) {
	int textW = 0;
	int textH = 0;

	for (auto& c : text) {
		auto ch = this->characters[c];

		textW += (ch->Advance >> 6);
		textH = ch->Size.y;
	}

	return glm::ivec2(textW, textH);
}

glm::ivec2 VideoSystem::getCenteredTextPosition(std::string& text, std::shared_ptr<Hitbox> rect) {
	glm::ivec2 stringDims = this->getTextDimensions(text);
	int rectW = 0;
	int rectH = 0;
	
	if (rect == NULL) {
		rectW = this->getWindowWidth();
		rectH = this->getWindowHeight(); 
	} else {
		rectW = rect->w;
		rectH = rect->h;
	}

	int textX = rectW / 2 - stringDims.x / 2;
	int textY = rectH / 2 - stringDims.y / 2;

	return glm::ivec2(textX, textY);
}