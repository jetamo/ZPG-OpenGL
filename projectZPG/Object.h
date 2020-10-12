#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"

class Object
{
private:
	GLuint VAO;
public:
	Object() 
	{

		float points[] = {
			0.0f,   0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
			0.5f,  -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		   -0.5f,  -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
		};

		//vertex buffer object (VBO)
		GLuint VBO = 0;
		glGenBuffers(1, &VBO); // generate the VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(points), points,
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
};

