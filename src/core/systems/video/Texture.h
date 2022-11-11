#pragma once

#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include "../../../stb/stb_image.h"

class Texture {
private:

public:
	Texture(std::string& filename);
};