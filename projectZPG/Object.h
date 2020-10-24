#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Shader.h"
#include <iostream>

class Object
{
private:
	GLuint VAO;

	void bind() {
		glBindVertexArray(VAO);
	}

	int pointsSize;
public:
	//float* points;
	Object(const float* _points, int _pointsSize)
	{
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

	void draw(Shader& shader) {
		bind();
		shader.bind();
		// draw triangles
		glDrawArrays(GL_TRIANGLES, 0, pointsSize); //mode,first,count
	}
};

