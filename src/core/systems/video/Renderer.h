#pragma once

#include <memory>
#include <vector>
#include <gl/glew.h>

#include "Shader.h"
#include "textures/Texture.h"
#include "views/View.h"

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

class Renderer {
private:
	std::unique_ptr<Quad> quad;
	glm::mat4 projectionMatrix;

public:
	Renderer(int windowWidth, int windowHeight);

	void draw(std::shared_ptr<Texture>& texture, std::shared_ptr<Shader>& shader);
	void draw(std::shared_ptr<IView>& view, std::shared_ptr<Shader>& shader);
};