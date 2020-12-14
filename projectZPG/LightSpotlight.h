#pragma once
#include "Light.h"

class LightSpotlight : public Light
{
private:
	glm::vec3 lightPos;
	float radius;
public:
	LightSpotlight(float _radius, glm::vec3 _position, glm::vec3 _color, LightType _type, int _index)
	{
		lightPos = _position;
		lightColor = _color;
		type = _type;
		index = _index;
		radius = _radius;
	}

	virtual void setUniforms(Shader* shader) {
		Light::setUniforms(shader);
		std::string lp = "lights[" + std::to_string(index) + "].lightPos";
		shader->setUniform(lp.c_str(), lightPos);
		std::string r = "lights[" + std::to_string(index) + "].radius";
		shader->setUniform(r.c_str(), radius);

	}
};

