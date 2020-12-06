#pragma once
#include <stdlib.h>
#include <string>
#include <GLFW/glfw3.h>
class Window
{
private:
	GLFWwindow* window;
public:
	Window(int xSize, int ySize, const char* name) {
		window = glfwCreateWindow(xSize, ySize, name, NULL, NULL);
		if (!window)
		{
			glfwTerminate();
			exit(EXIT_FAILURE);
		}
		glfwMakeContextCurrent(window);
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		float ratio = width / (float)height;
		glViewport(0, 0, width, height);


	}

	GLFWwindow* getWindow() {
		return this->window;
	}
};

