#pragma once

#include <gl/glew.h>

class Texture {
private:
    int width;
    int height;
    int channels;

	GLuint textureId;

public:
	Texture(GLuint textureId, int width, int height, int channels);

    int getWidth();
    int getHeight();
    int getChannels();
    GLuint getTextureId();
};