#pragma once
#include "Light.h"

class LightDirectional : public Light
{
private:
	glm::vec3 direction;
public:
	LightDirectional(glm::vec3 _direction, glm::vec3 _color, LightType _type, int _index)
	{
		direction = _direction;
		lightColor = _color;
		type = _type;
		index = _index;
	}

	virtual void setUniforms(Shader* shader) {
		Light::setUniforms(shader);
		std::string ld = "lights[" + std::to_string(index) + "].direction";
		shader->setUniform(ld.c_str(), direction);
	}
};

