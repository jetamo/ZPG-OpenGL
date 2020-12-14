#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include "soil/SOIL.h"
#include <string>
#include "glm/mat4x4.hpp"
#include "Observer.h"
#include "ShaderLoader/ShaderLoader.h"

class Shader : public Observer
{
private:
	GLuint shader;
	GLuint textureID;
public:
	Shader(const char* vertex_shader, const char* fragment_shader) {
		ShaderLoader * loader = new ShaderLoader();
		shader = loader->loadShader(vertex_shader, fragment_shader);
		glLinkProgram(shader);
	}
	unsigned int getID() {
		return shader;
	}

	//void createTexture(const char* picture) {
	//	//Bind the first texture to the first texture unit.
	//	glActiveTexture(GL_TEXTURE0);
	//
	//	textureID = SOIL_load_OGL_texture(picture, SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
	//
	//	glBindTexture(GL_TEXTURE_2D, textureID);
	//
	//	//Set texture unit to fragment shader
	//	GLint uniformID = glGetUniformLocation(shader, "textureUnitID");
	//	glUniform1i(uniformID, 0);
	//}
	//
	//void bindTexture() {
	//	glBindTexture(GL_TEXTURE_2D, textureID);
	//
	//	//Set texture unit to fragment shader
	//	GLint uniformID = glGetUniformLocation(shader, "textureUnitID");
	//	glUniform1i(uniformID, 0);
	//}

	void bind() {
		glUseProgram(shader);
	}

	void setUniform(const char* name, const glm::mat4& matrix) {
		glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, &matrix[0][0]);
	}

	void setUniform(const char* name, const glm::vec3& vector) {
		glUniform3f(glGetUniformLocation(shader, name), vector.x, vector.y, vector.z);
	}
	void setUniform(const char* name, const int number) {
		glUniform1i(glGetUniformLocation(shader, name), number);
	}
	void setUniform(const char* name, const float number) {
		glUniform1i(glGetUniformLocation(shader, name), number);
	}

	void update(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 position, glm::vec3 direction) {
		bind();
		setUniform("view", viewMatrix);
		setUniform("projection", projectionMatrix);
		setUniform("camPosition", position);
		setUniform("camDirection", direction);
	}
};

