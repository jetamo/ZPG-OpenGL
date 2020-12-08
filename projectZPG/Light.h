#pragma once
#include "glm/vec3.hpp"
class Light
{
public:
	glm::vec3 lightPos;
	glm::vec3 lightColor;
	int type;
	Light(glm::vec3 _position, glm::vec3 _color, int _type) 
	{
		lightPos = _position;
		lightColor = _color;
		type = _type;
	}
	void changeLightPos(glm::vec3 _position)
	{
		lightPos = _position;
	}

	void changeLightColor(glm::vec3 _color)
	{
		lightColor = _color;
	}
};

