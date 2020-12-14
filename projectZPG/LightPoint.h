#pragma once
#include "Light.h"

class LightPoint : public Light
{
private:
	glm::vec3 lightPos;

public:
	LightPoint(glm::vec3 _position, glm::vec3 _color, LightType _type, int _index)
	{
		lightPos = _position;
		lightColor = _color;
		type = _type;
		index = _index;
	}

	virtual void setUniforms(Shader* shader) {
		Light::setUniforms(shader);
		std::string lp = "lights[" + std::to_string(index) + "].lightPos";
		shader->setUniform(lp.c_str(), lightPos);
	}
};

