#pragma once
#include "glm/vec3.hpp"
#include "Shader.h"
#include <string>

enum LightType {
	Point = 1,
	Spotlight = 2,
	Directional = 3
};

class Light 
{
protected:
	glm::vec3 lightColor;
	LightType type;
	int index;
public:
	void changeLightColor(glm::vec3 _color)
	{
		lightColor = _color;
	}

	virtual void setUniforms(Shader* shader) {
		std::string lc = "lights[" + std::to_string(index) + "].lightColor";
		std::string t = "lights[" + std::to_string(index) + "].type";
		shader->setUniform(lc.c_str(), lightColor);
		shader->setUniform(t.c_str(), type);
	}
};

