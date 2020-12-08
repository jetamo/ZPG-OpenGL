#pragma once
#include "Object.h"
#include "Texture.h"

class TextureObject : public Object
{
public:
	//Texture* texture;

	TextureObject(const float* _points, int _pointsSize, Shader* _shader, int _id) : Object(_points, _pointsSize, _shader, _id)
	{
		texture = new Texture();
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
		glEnableVertexAttribArray(1); //enable vertex attributes
		glEnableVertexAttribArray(2); //enable vertex attributes
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, NULL);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid*)(6 * sizeof(float)));

	}
	
	void setTexture(Texture* _texture) {
		texture = _texture;
	}
};

