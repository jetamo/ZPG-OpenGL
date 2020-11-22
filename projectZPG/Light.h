#pragma once
#include "glm/vec3.hpp"
class Light
{
public:
	glm::vec3 lightPos;
	glm::vec3 lightColor;
	Light(glm::vec3 _position, glm::vec3 _color) 
	{
		lightPos = _position;
		lightColor = _color;
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

