#pragma once

#include "glm/mat4x4.hpp"

class Observer
{
public:
	virtual void update(glm::mat4 viewMatrix, glm::mat4 projectionMatrix, glm::vec3 camPosition) = 0;
};

