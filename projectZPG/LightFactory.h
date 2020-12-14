#pragma once
#include "Light.h"
#include "LightPoint.h"
#include "LightSpotlight.h"
#include "LightDirectional.h"

class LightFactory
{
public:
	LightPoint* getLightPoint(glm::vec3 _position, glm::vec3 _color, int _index) {
		return new LightPoint(_position, _color, LightType::Point, _index);
	}
	LightSpotlight* getLightSpotlight(float _radius, glm::vec3 _position, glm::vec3 _color, int _index) {
		return new LightSpotlight(_radius, _position, _color, LightType::Spotlight, _index);
	}
	LightDirectional* getLightDirectional(glm::vec3 _direction, glm::vec3 _color, int _index) {
		return new LightDirectional(_direction, _color, LightType::Directional, _index);
	}
};

