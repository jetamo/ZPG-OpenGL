#pragma once
#include "Object.h"
#include "Texture.h"
#include "CurveMover.h"

class TextureObject : public Object, CurveMover
{
private:
public:
	Texture* texture;

	TextureObject(const void* _points, int _pointsSize, Shader* _shader, int _id, int mNumFaces, unsigned int* pIndices) : Object(_points, _pointsSize, _shader, _id, mNumFaces, pIndices)
	{
		texture = new Texture();
		id = _id;
		shader = _shader;
		pointsSize = _pointsSize;
		count = sizeof(GLuint) * mNumFaces * 3;
		const void* points = _points;


		//vertex buffer object (VBO)
		GLuint VBO = 0;
		glGenBuffers(1, &VBO); // generate the VBO
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, pointsSize, points,
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


		IBO = 0;

		glGenBuffers(1, &IBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, pIndices, GL_STATIC_DRAW);
	}
	
	void setTexture(Texture* _texture) {
		texture = _texture;
	}

	virtual void moveOnCurve() {
		CurveMover::moveOnCurve();
		this->setPosition(final);
	}
};

