#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <iostream>



//SOIL_load_OGL_texture("test.png", SOIL_LOAD_RGBA, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);

class Object
{
private:
protected:
	GLuint VAO;
	int id;
public:
	int pointsSize;
	Shader* shader;
	glm::mat4 transform = glm::mat4(1.0f);
	//float* points;
	Object(const float* _points, int _pointsSize, Shader* _shader, int _id)
	{
		id = _id;
		shader = _shader;
		pointsSize = _pointsSize;
		const float* points = _points;

		//vertex buffer object (VBO)
		GLuint VBO = 0;
		glGenBuffers(1, &VBO); // generate the VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, pointsSize, &points[0],
			GL_STATIC_DRAW);
		//vertex attribute object(VAO)
		VAO = 0;
		glGenVertexArrays(1, &VAO); //generate the VAO
		glBindVertexArray(VAO); //bind the VAO
		glEnableVertexAttribArray(0); //enable vertex attributes
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, NULL);

		glEnableVertexAttribArray(1); //enable vertex attributes
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3 * sizeof(float)));
	}

	void bind() {
		glBindVertexArray(VAO);
	}

	void setPosition(glm::vec3 newPosition)
	{
		transform = glm::translate(glm::mat4(1.0), newPosition);
	}
	 
	void rotate(float fi, glm::vec3 axis)
	{
		transform = glm::rotate(transform, glm::radians(fi), axis);
	}

	int getId() {
		return id;
	}
};

