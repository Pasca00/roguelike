#include "TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../../../stb/stb_image.h"

const GLint pixelFormat[5] = { 0, GL_RED, GL_RG, GL_RGB, GL_RGBA };
const GLint internalFormat[][5] = {
	{ 0, GL_R8, GL_RG8, GL_RGB8, GL_RGBA8 },
	{ 0, GL_R16, GL_RG16, GL_RGB16, GL_RGBA16 },
	{ 0, GL_R16F, GL_RG16F, GL_RGB16F, GL_RGBA16F },
	{ 0, GL_R32F, GL_RG32F, GL_RGB32F, GL_RGBA32F }
};

TextureManager::TextureManager() {
	this->targetType = GL_TEXTURE_2D;
	this->wrappingMode = GL_REPEAT;
	this->textureMinFilter = GL_NEAREST_MIPMAP_NEAREST;
	this->textureMagFilter = GL_NEAREST;

	stbi_set_flip_vertically_on_load(true);
}

unsigned char* TextureManager::loadImageData(std::string& filename) {
	return stbi_load(filename.c_str(), &this->width, &this->height, &this->channels, 0);
}

std::shared_ptr<Texture> TextureManager::makeTexture(unsigned char* textureData, int width, int height, int channels) {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(targetType, 0, internalFormat[0][channels], width, height, 0, pixelFormat[channels], GL_UNSIGNED_BYTE, textureData);
	
	glGenerateMipmap(targetType);
	glBindTexture(targetType, 0);

	stbi_image_free(textureData);

	return std::make_shared<Texture>(textureID, width, height, channels);
}

std::shared_ptr<Texture> TextureManager::getSingleTextureFromFile(std::string filename) {
	auto data = this->loadImageData(filename);
	if (data == NULL) {
		std::cout << "FAILED TO LOAD IMAGE DATA FOR :: " << filename << "\n";
		return NULL;
	}

	return this->makeTexture(data, width, height, channels);
}

std::vector<std::vector<std::shared_ptr<Texture>>> TextureManager::getTexturesFromSpriteSheet(
	std::string filename,
	std::vector<int> framesPerSprite,
	unsigned int spriteWidth,
	unsigned int spriteHeight
) {
	auto spritesheetData = this->loadImageData(filename);
	if (spritesheetData == NULL) {
		std::cout << "FAILED TO LOAD IMAGE DATA FOR :: " << filename << "\n";
		return {};
	}

	unsigned long size = spriteWidth * spriteHeight * channels;
	std::vector<std::vector<std::shared_ptr<Texture>>> sprites(framesPerSprite.size());
	for (int sprite = 0; sprite < framesPerSprite.size(); sprite++) {
		std::vector<std::shared_ptr<Texture>> frames(framesPerSprite[sprite]);
		for (int frame = 0; frame < framesPerSprite[sprite]; frame++) {
			unsigned char* data = new unsigned char[size];
			for (int row = 0; row < spriteHeight; row++) {
				memcpy(
					data + row * spriteWidth * channels, 
					spritesheetData + row * width * channels + frame * spriteWidth * channels + sprite * width * spriteHeight * channels,
					spriteWidth * channels
				);
			}

			frames[frame] = this->makeTexture(data, spriteWidth, spriteHeight, channels);
		}

		// The image pixels are stored bottom-up 
		// so we need to reverse the order
		sprites[framesPerSprite.size() - sprite - 1] = frames;
	}
	
	stbi_image_free(spritesheetData);

	return sprites;
}

std::shared_ptr<Texture> TextureManager::makeFramebufferTexture(int width, int height, int channels) {
	unsigned int textureId;
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureId, 0);

	return std::make_shared<Texture>(textureId, width, height, channels);
}
