#include "Texture.h"

Texture::Texture(
	GLuint textureId, 
	int width, 
	int height, 
	int channels
) : textureId(textureId), 
	width(width), 
	height(height), 
	channels(channels) { }

Texture::~Texture() {
	glDeleteTextures(1, &this->textureId);
}

int Texture::getWidth()   {
	return this->width;
}

int Texture::getHeight()  {
	return this->height;
}

int Texture::getChannels() {
	return this->channels;
}

GLuint Texture::getTextureId() {
	return this->textureId;
}