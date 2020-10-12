#pragma once

#include "glm/gtc/matrix_transform.hpp"

class Camera
{
private:
	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	glm::mat4 Projection;

	// Camera matrix
	glm::mat4 View;

	glm::vec3 target = glm::vec3(0, 0, -1);

	glm::vec3 position = glm::vec3(4, 0, -3);
	
public:
	Camera()
	{
		setCamera();
		Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);

	}

	void setCamera() {
		View = glm::lookAt(
			position, // Camera is at (4,3,-3), in World Space
			glm::vec3(target.x, target.y, target.z) + position, // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
	}

	glm::mat4 getProjection() {
		return Projection;
	}

	glm::mat4 getView() {
		return View;
	}

	void changeViewAngle(double deltaX) {
		glm::mat4 rotationMat = glm::rotate(glm::mat4(1.0f), glm::radians((float)deltaX), { 0, 1, 0 }); //0, 1, 0 == upVector
		target = glm::mat3(rotationMat) * target;
		setCamera();
	}
};

