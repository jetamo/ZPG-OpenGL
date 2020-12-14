#pragma once
#include "glm/vec3.hpp"
#include "Shader.h"

class Light 
{
public:
	glm::vec3 lightPos;
	glm::vec3 lightColor;
	float radius;
	int type;
	Light(glm::vec3 _position, glm::vec3 _color, int _type) 
	{
		lightPos = _position;
		lightColor = _color;
		type = _type;
		radius = 0;
	}

	Light(glm::vec3 _position, glm::vec3 _color, int _type, float _radius) 
	{
		lightPos = _position;
		lightColor = _color;
		type = _type;
		radius = _radius;
	}

	void changeLightPos(glm::vec3 _position)
	{
		lightPos = _position;
	}

	void changeLightColor(glm::vec3 _color)
	{
		lightColor = _color;
	}

	void setUniforms(Shader* shader, int index) {
		shader->setUniform("lights[0].lightPos", lightPos);
		shader->setUniform("lights[0].lightColor", lightColor);
		shader->setUniform("lights[0].radius", radius);
		shader->setUniform("lights[0].type", type);
	}
};

