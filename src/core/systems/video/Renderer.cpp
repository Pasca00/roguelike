#include "Renderer.h"

Quad::Quad(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices) {
	this->vertices = vertices;
	this->indices = indices;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, &VBO[0]);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

	if (glGetError()) {
		printf("ERROR GENERATING BUFFERS\n");
	}
}

unsigned int Quad::getVAO() {
	return VAO;
}

std::vector<unsigned int> Quad::getIndices() {
	return indices;
}

/* ------------ Renderer methods ------------ */
Renderer::Renderer(int windowHeight, int windowWidth) {
	std::vector<Vertex> vertices = {
		Vertex(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1), glm::vec2(0.f, 0.f)),
		Vertex(glm::vec3(0.f, 1.f, 0.f), glm::vec3(1), glm::vec2(0.f, 1.f)),
		Vertex(glm::vec3(1.f, 1.f, 0.f), glm::vec3(1), glm::vec2(1.f, 1.f)),
		Vertex(glm::vec3(1.f, 0.f, 0.f), glm::vec3(1), glm::vec2(1.f, 0.f))
	};

	std::vector<unsigned int> indices = {
		0, 1, 2, 3
	};

	quad = std::make_unique<Quad>(vertices, indices);

	projectionMatrix = glm::ortho(0.f, (float)windowWidth, 0.f, (float)windowHeight, -0.01f, 10.f);
}

void Renderer::draw(std::shared_ptr<Texture>& texture, std::shared_ptr<Shader>& shader) {
	glm::mat4 modelMatrix(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(texture->getWidth() * 2, texture->getHeight() * 2, 1));

	shader->use();
	shader->setModelMatrix(modelMatrix);
	shader->setProjectionMatrix(projectionMatrix);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getTextureId());

	glBindVertexArray(quad->getVAO());

	glDrawArrays(GL_QUADS, 0, quad->getIndices().size());

	glBindVertexArray(0);
}