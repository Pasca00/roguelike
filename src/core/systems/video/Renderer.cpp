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

Character::Character(unsigned int TextureID, glm::ivec2 Size, glm::ivec2 Bearing, unsigned int Advance)
	: TextureID(TextureID), Size(Size), Bearing(Bearing), Advance(Advance) {}

/* ------------ Renderer methods ------------ */
Renderer::Renderer(int windowHeight, int windowWidth, Character** characters) {
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

	projectionMatrix = glm::ortho(0.f, static_cast<float>(windowWidth), 0.f, static_cast<float>(windowHeight), -0.01f, 10.f);

	this->characters = characters;
}

void Renderer::draw(
	std::shared_ptr<Texture>& texture, 
	std::shared_ptr<Shader>& shader,
	std::unordered_map<std::string, unsigned int>& uintUniforms,
	std::unordered_map<std::string, int>& intUniforms,
	std::unordered_map<std::string, float>& floatUniforms
) {
	glm::mat4 modelMatrix(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(texture->getWidth(), texture->getHeight(), 1));

	shader->use();
	shader->setModelMatrix(modelMatrix);
	shader->setProjectionMatrix(this->projectionMatrix);

	this->setUintUniforms(shader, uintUniforms);
	this->setIntUniforms(shader, intUniforms);
	this->setFloatUniforms(shader, floatUniforms);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture->getTextureId());

	glBindVertexArray(quad->getVAO());

	glDrawArrays(GL_QUADS, 0, quad->getIndices().size());

	glBindVertexArray(0);
}

void Renderer::draw(
	std::shared_ptr<IView>& view,
	std::shared_ptr<Shader>& shader,
	glm::mat4& viewMatrix,
	std::unordered_map<std::string, unsigned int>& uintUniforms,
	std::unordered_map<std::string, int>& intUniforms,
	std::unordered_map<std::string, float>& floatUniforms
) {
	glm::mat4 modelMatrix(1);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(view->getX(), view->getY(), 0));
	modelMatrix = glm::scale(modelMatrix, glm::vec3(view->getWidth() * view->getSize(), view->getHeight() * view->getSize(), 1));

	shader->use();
	shader->setModelMatrix(modelMatrix);
	shader->setProjectionMatrix(this->projectionMatrix);
	shader->setViewMatrix(viewMatrix);

	glUniform1i(shader->getUniformLocation("render_flipped"), view->isFlipped() ? 1 : 0);

	auto c = view->getOverlayColor();
	glUniform4fv(shader->getUniformLocation("overlay_color"), 1, &c[0]);

	this->setUintUniforms(shader, uintUniforms);
	this->setIntUniforms(shader, intUniforms);
	this->setFloatUniforms(shader, floatUniforms);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, view->getTextureId());

	glBindVertexArray(quad->getVAO());

	glDrawArrays(GL_QUADS, 0, quad->getIndices().size());

	glBindVertexArray(0);

	/*float f;
	glGetUniformfv(shader->getProgram(), shader->getUniformLocation("playerX"), &f);
	printf("x: %f\n", f);*/
}

void Renderer::drawText(
	std::shared_ptr<TextView>& view,
	std::shared_ptr<Shader>& shader,
	std::unordered_map<std::string, unsigned int>& uintUniforms,
	std::unordered_map<std::string, int>& intUniforms,
	std::unordered_map<std::string, float>& floatUniforms
) {
	float x = view->getX();

	auto& text = view->getText();
	for (auto c : text) {
		Character* ch = this->characters[c];
		float xpos = x + ch->Bearing.x * view->getSize();
		float ypos = view->getY() - (ch->Size.y - ch->Bearing.y) * view->getSize();

		float w = ch->Size.x * view->getSize();
		float h = ch->Size.y * view->getSize();

		glm::mat4 modelMatrix(1);
		modelMatrix = glm::translate(modelMatrix, glm::vec3(xpos, ypos, 0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(w, h, 1));

		shader->use();
		shader->setModelMatrix(modelMatrix);
		shader->setProjectionMatrix(this->projectionMatrix);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, ch->TextureID);

		glBindVertexArray(quad->getVAO());
		glDrawArrays(GL_QUADS, 0, quad->getIndices().size());

		x += (ch->Advance >> 6) * view->getSize();
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Renderer::setUintUniforms(std::shared_ptr<Shader>& shader, std::unordered_map<std::string, unsigned int>& uniforms) {
	for (const auto& u : uniforms) {
		glUniform1ui(shader->getUniformLocation(u.first), u.second);
	}
}

void Renderer::setIntUniforms(std::shared_ptr<Shader>& shader, std::unordered_map<std::string, int>& uniforms) {
	for (const auto& u : uniforms) {
		glUniform1i(shader->getUniformLocation(u.first), u.second);
	}
}

void Renderer::setFloatUniforms(std::shared_ptr<Shader>& shader, std::unordered_map<std::string, float>& uniforms) {
	for (const auto& u : uniforms) {
		glUniform1f(shader->getUniformLocation(u.first), u.second);
	}
}