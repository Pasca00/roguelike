#include "Framebuffer.h"

Framebuffer::Framebuffer(GLint window_fbo, std::shared_ptr<TextureManager>& textureManager, int width, int height, int channels)
	: width(width), height(height) {
	this->window_fbo = window_fbo;

	glGenFramebuffers(1, &this->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);

	auto texture = textureManager->makeFramebufferTexture(width, height, channels);
	this->framebufferView = std::make_shared<View>(texture);

	if (glCheckNamedFramebufferStatus(this->fbo, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "FRAMEBUFFER NOT COMPLETE\n";
	} else {
		std::cout << "FRAMEBUFFER COMPLETE\n";
	}

	this->unbind();
}

Framebuffer::~Framebuffer() {
	glDeleteFramebuffers(1, &this->fbo);
}

void Framebuffer::bind(bool clearBuffer) {
	glBindFramebuffer(GL_FRAMEBUFFER, this->fbo);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);
	if (clearBuffer) {
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
}

void Framebuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, this->window_fbo);
	glDisable(GL_DEPTH_TEST);
}

std::shared_ptr<View> Framebuffer::getView() {
	return this->framebufferView;
}