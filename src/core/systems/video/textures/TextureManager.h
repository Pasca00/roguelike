#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <iostream>

#define GLEW_STATIC
#include <gl/glew.h>

#include "Texture.h"

class TextureManager {
private:
    int width;
    int height = 0;
    int channels = 0;
    int textureID = 0;

	GLuint targetType;

	GLenum wrappingMode;

	GLenum textureMinFilter;
	GLenum textureMagFilter;

	unsigned char* loadImageData(std::string& filename);
	std::shared_ptr<Texture> makeTexture(unsigned char* textureData, int width, int height, int channels);

public:
	TextureManager();

	std::shared_ptr<Texture> getSingleTextureFromFile(std::string filename);
	std::vector<std::vector<std::shared_ptr<Texture>>> getTexturesFromSpriteSheet(
		std::string filename, 
		std::vector<int> framesPerSprite,
		unsigned int spriteWidth = 32, 
		unsigned int spriteHeight = 32
	);
};