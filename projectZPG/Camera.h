#pragma once

#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "Subject.h"
#include <iostream>

class Camera : public Subject
{
private:

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 uniunit <-> 100 units
	glm::mat4 Projection;

	// Camera matrix
	glm::mat4 View;

	glm::vec3 target = glm::vec3(0, 0, 0);

	glm::vec3 position = glm::vec3(0.f, 0.f, -3.f);

	glm::vec3 up = glm::vec3(0.f, 1.f, 0.f);

	const float cameraSpeed = 0.05f;
		
	float fi = 0.f;
	float psi = 0.f;
	
public:
	Camera()
	{
		setCamera();
		Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.01f, 100.0f);

	}

	void setCamera() {
		View = glm::lookAt(
			position, // Camera is at (4,3,-3), in World Space
			glm::vec3(target.x, target.y, target.z) + position, // and looks at the origin
			glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
		);
		notifyObservers();
	}

	glm::mat4 getProjection() {
		return Projection;
	}

	glm::mat4 getView() {
		return View;
	}

	void moveCamera(int key) {
	}

	void changeViewAngle(double deltaX, double deltaY) {


		fi += deltaX;
		psi += deltaY;
		//if (deltaX != 0)
		//{
		//	fi += deltaX / glm::abs(deltaX);
		//	std::cout << fi << "; " << psi << std::endl;
		//}
		//if (deltaY != 0)
		//{
		//	psi += deltaY / glm::abs(deltaY);\
		//		std::cout << fi << "; " << psi << std::endl;
		//}

		std::cout << fi << "; " << psi << std::endl;
		if (psi > 89.0f)
			psi = 89.0f;
		if (psi < -89.0f)
			psi = -89.0f;

		//if (glm::sin(glm::radians((float)psi)) < -0.9999f)
		//{
		//	psi = 999;
		//}
		//if (glm::sin(glm::radians((float)psi)) > 0.9999f)
		//	psi = 999;

		target.x = glm::cos(glm::radians((float)fi));
		target.z = glm::sin(glm::radians((float)fi));
		target.y = glm::sin(glm::radians((float)psi));
		
		target = glm::normalize(target);
		setCamera();
	}

	void checkForMovement(GLFWwindow* window) {
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			position += cameraSpeed * target;
			notifyObservers();
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			position -= cameraSpeed * target;
			notifyObservers();
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			position -= glm::normalize(glm::cross(target, up)) * cameraSpeed;
			notifyObservers();
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			position += glm::normalize(glm::cross(target, up)) * cameraSpeed;
			notifyObservers();
		}
	}

	void notifyObservers() {
		for (Observer* observer : observerCollection)
		{
			observer->update(View, Projection, position);
		}
	}
};

