#include "Shader.h"

Shader::Shader(const char* basePath, std::string name) {
	this->name = name;

	std::string vertexCode;
	std::string fragmentCode;

	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	std::string fullPath = std::string(basePath).append(name);

	std::string vFileName = fullPath;
	std::string fFileName = fullPath;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		vShaderFile.open(vFileName.append("VertexShader.glsl"));
		fShaderFile.open(fFileName.append("FragmentShader.glsl"));

		std::stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	} catch (std::ifstream::failure e) {
		printf("ERROR OPENING SHADER FILE::%s\n", name.c_str());
	}

	vShaderCode = vertexCode.c_str();
	fShaderCode = fragmentCode.c_str();

	this->compile();
}

void Shader::compile() {
	GLuint vertex, fragment;

	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		printf("ERROR COMPILING VERTEX SHADER::%s::%s\n", name.c_str(), infoLog);
	}

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		printf("ERROR COMPILING VERTEX SHADER::%s::%s\n", name.c_str(), infoLog);
	}

	program = glCreateProgram();
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		printf("ERROR LINKING SHADERS::%s::%s\n", name.c_str(), infoLog);
	}
	else {
		printf("SHADER COMPILED SUCCESSFULLY::%s\n", name.c_str());
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(program);
}

GLint Shader::getUniformLocation(const char* name) {
	return glGetUniformLocation(program, name);
}

GLint Shader::getUniformLocation(const std::string& name) {
	return glGetUniformLocation(program, name.c_str());
}

GLuint Shader::getProgram() {
	return program;
}

void Shader::setModelMatrix(glm::mat4& modelMatrix) {
	glUniformMatrix4fv(getUniformLocation("Model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void Shader::setProjectionMatrix(glm::mat4& projectionMatrix) {
	glUniformMatrix4fv(getUniformLocation("Projection"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
}

void Shader::setTimeUniform(float time) {
	glUniform1f(getUniformLocation("time"), time);
}