#pragma once
#include "GL/glew.h"
#include <GLFW/glfw3.h>
#include <string>
#include "glm/mat4x4.hpp"
#include "Observer.h"

class Shader : public Observer
{
private:
	unsigned int shader;
public:
	Shader(const char* vertex_shader, const char* fragment_shader) {

		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, &vertex_shader, NULL);
		glCompileShader(vertexShader);

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
		glCompileShader(fragmentShader);

		shader = glCreateProgram();
		glAttachShader(shader, fragmentShader);
		glAttachShader(shader, vertexShader);
		glLinkProgram(shader);

		GLint status;
		glGetProgramiv(shader, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLogLength;
			glGetProgramiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
			GLchar* strInfoLog = new GLchar[infoLogLength + 1];
			glGetProgramInfoLog(shader, infoLogLength, NULL, strInfoLog);
			fprintf(stderr, "Linker failure: %s\n", strInfoLog);
			delete[] strInfoLog;
		}
	}

	void bind() {
		glUseProgram(shader);
	}

	void setUniform(const char* name, const glm::mat4& matrix) {
		glUniformMatrix4fv(glGetUniformLocation(shader, name), 1, GL_FALSE, &matrix[0][0]);
	}

	void setUniform(const char* name, const glm::vec3& vector) {
		glUniform3f(glGetUniformLocation(shader, name), vector.x, vector.y, vector.z);
	}

	void update(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 position) {
		setUniform("view", viewMatrix);
		setUniform("projection", projectionMatrix);
		setUniform("camPosition", position);
	}
};

