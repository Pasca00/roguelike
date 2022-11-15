#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include <gl/glew.h>

#include "../../../glm/glm.hpp"
#include "../../../glm/gtc/type_ptr.hpp"

class Shader {
private:
	GLuint program;

	std::string name;
	const char* vShaderCode;
	const char* fShaderCode;

public:
	Shader(const char* basePath, std::string name);

	void use();
	void compile();

	GLint getUniformLocation(const char* name);

	void setModelMatrix(glm::mat4& modelMatrix);
	void setProjectionMatrix(glm::mat4& projectionMatrix);

	void setTimeUniform(float time);

	GLuint getProgram();

};