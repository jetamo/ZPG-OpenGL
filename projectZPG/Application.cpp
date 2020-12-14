#include <stdlib.h>
#include <stdio.h>

#include "GL/glew.h"
#include <GLFW/glfw3.h>  

//Include GLM  
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "Application.h"
#include "CallbacksHandler.h"
#include "Shader.h"
#include "Object.h"
#include "TextureObject.h"
#include "Camera.h"
#include "Light.h"
#include "Renderer.h"
#include <vector>

#include "Window.h"
#include "ObjectManager.h"
#include "Texture.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Game.h"


Application Application::instance;
bool Application::clicked = false;


void Application::Start()
{
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}

	Window* window = new Window(800, 600, "ZPG");
	Application::setCallbacks(window->getWindow());

	glfwSwapInterval(1);
	// start GLEW extension handler
	glewExperimental = GL_TRUE;
	glewInit();
	// get version info
	printf("OpenGL Version: %s\n", glGetString(GL_VERSION));
	printf("Using GLEW %s\n", glewGetString(GLEW_VERSION));
	printf("Vendor %s\n", glGetString(GL_VENDOR));
	printf("Renderer %s\n", glGetString(GL_RENDERER));
	printf("GLSL %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));
	int major, minor, revision;
	glfwGetVersion(&major, &minor, &revision);
	printf("Using GLFW %i.%i.%i\n", major, minor, revision);

	Game* game = new Game(window);

	game->run();

	glfwDestroyWindow(window->getWindow());
	glfwTerminate();
	exit(EXIT_SUCCESS);

}

void Application::setCallbacks(GLFWwindow* _window)
{
		glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double mouseXPos, double mouseYPos)                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                          
			-> void {CallbacksHandler::getInstance().cursor_pos_callback(window, mouseXPos, mouseYPos); });

		glfwSetErrorCallback([](int error, const char* description)
			-> void {CallbacksHandler::getInstance().error_callback(error, description); });

		glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			-> void {CallbacksHandler::getInstance().key_callback(window, key, scancode, action, mods); });


		glfwSetMouseButtonCallback(_window, [](GLFWwindow* window, int button, int action, int mode)
			-> void {CallbacksHandler::getInstance().button_callback(window, button, action, mode); });

		glfwSetWindowFocusCallback(_window, [](GLFWwindow* window, int focused)
			-> void {CallbacksHandler::getInstance().window_focus_callback(window, focused); });

		glfwSetWindowIconifyCallback(_window, [](GLFWwindow* window, int iconified)
			-> void {CallbacksHandler::getInstance().window_iconify_callback(window, iconified); });

		glfwSetWindowSizeCallback(_window, [](GLFWwindow* window, int width, int height)
			-> void {CallbacksHandler::getInstance().window_size_callback(window, width, height); });
}


