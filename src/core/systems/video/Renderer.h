#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <gl/glew.h>

#include "Shader.h"
#include "textures/Texture.h"
#include "views/View.h"
#include "views/TextView.h"

#include "../../../glm/glm.hpp"
#include "../../../glm/gtc/matrix_transform.hpp"

struct Vertex {
	Vertex(glm::vec3 position, glm::vec3 color = glm::vec3(1.f),
		glm::vec2 texCoord = glm::vec2(0))
		: position(position), color(color), texCoord(texCoord) {}

	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoord;
};

class Quad {
private:
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	GLuint VAO;
	GLuint VBO[2];

public:
	Quad(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices);
	unsigned int getVAO();

	std::vector<unsigned int> getIndices();
};

struct Character {
	unsigned int TextureID;
	glm::ivec2   Size;
	glm::ivec2   Bearing;
	unsigned int Advance;

	Character(unsigned int TextureID, glm::ivec2 Size, glm::ivec2 Bearing, unsigned int Advance);
};

class Renderer {
private:
	std::unique_ptr<Quad> quad;
	glm::mat4 projectionMatrix;

	Character** characters;

	void setUintUniforms(std::shared_ptr<Shader>& shader, std::unordered_map<std::string, unsigned int>& uniforms);
	void setIntUniforms(std::shared_ptr<Shader>& shader, std::unordered_map<std::string, int>& uniforms);
	void setFloatUniforms(std::shared_ptr<Shader>& shader, std::unordered_map<std::string, float>& uniforms);

public:
	Renderer(int windowWidth, int windowHeight, Character** characters);

	void draw(
		std::shared_ptr<Texture>& texture, 
		std::shared_ptr<Shader>& shader,
		std::unordered_map<std::string, unsigned int>& uintUniforms,
		std::unordered_map<std::string, int>& intUniforms,
		std::unordered_map<std::string, float>& floatUniforms
	);
	void draw(
		std::shared_ptr<IView>& view, 
		std::shared_ptr<Shader>& shader,
		glm::mat4& viewMatrix,
		std::unordered_map<std::string, unsigned int>& uintUniforms,
		std::unordered_map<std::string, int>& intUniforms,
		std::unordered_map<std::string, float>& floatUniforms
	);
	void drawText(
		std::shared_ptr<TextView>& view,
		std::shared_ptr<Shader>& shader,
		std::unordered_map<std::string, unsigned int>& uintUniforms,
		std::unordered_map<std::string, int>& intUniforms,
		std::unordered_map<std::string, float>& floatUniforms
	);
};