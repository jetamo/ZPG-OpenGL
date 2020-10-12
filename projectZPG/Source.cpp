//Include GLFW  

//Include the standard C++ headers  
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
#include "Shader.h"
#include "Object.h"
#include "Camera.h"

const char* vertex_shader =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"layout(location=1) in vec3 color;"
"uniform mat4 model;"
"uniform mat4 view;"
"uniform mat4 projection;"
"out vec3 v_color;"
"void main () {"
"     gl_Position = (projection * view * model) * vec4(vp, 1.0f);"
"      v_color = color;"
"}";

const char* fragment_shader =
"#version 330\n"
"in vec3 v_color;"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4(v_color, 1.0);"
"}";


static double mouseX = 0;
static double mouseY = 0;

static double oldMouseX = 0;
static double oldMouseY = 0;

Application* app = new Application();

//GLM test

int main(void)
{
	GLFWwindow* window;
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}

	/*
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//*/
	window = glfwCreateWindow(800, 600, "ZPG", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
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
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	float ratio = width / (float)height;
	glViewport(0, 0, width, height);

	glfwSetCursorPosCallback(window, [](GLFWwindow* window, double mouseXPos, double mouseYPos)
		-> void {Application::getInstance().cursor_pos_callback(window, mouseXPos, mouseYPos); });

	glfwSetErrorCallback([](int error, const char* description)
		-> void {Application::getInstance().error_callback(error, description); });

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		-> void {Application::getInstance().key_callback(window, key, scancode, action, mods); });
	
	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mode)
		-> void {Application::getInstance().button_callback(window, button, action, mode); });

	glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused)
		-> void {Application::getInstance().window_focus_callback(window, focused); });

	glfwSetWindowIconifyCallback(window, [](GLFWwindow* window, int iconified)
		-> void {Application::getInstance().window_iconify_callback(window, iconified); });

	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		-> void {Application::getInstance().window_size_callback(window, width, height); });

	//create and compile shaders
	Shader* shader = new Shader(vertex_shader, fragment_shader);
	Object* object = new Object();
	Camera* camera = new Camera();


	glm::mat4 model;
	float rot = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		model = glm::rotate(glm::mat4(1.0f), glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::translate(glm::mat4(1.0f), { 0, 0, 0 })
		//	* glm::rotate(glm::mat4(1.0f), glm::radians(rot), { 0, 0, 1 })
		//	* glm::scale(glm::mat4(1.0f), { 1, 1, 1 });
		rot += 0.1f;
		oldMouseX = mouseX;
		oldMouseY = mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		// clear color and depth buffer
		glm::dvec2 delta = (glm::dvec2(oldMouseX, oldMouseY) - glm::dvec2(mouseX, mouseY));
		camera->changeViewAngle(delta.x / 2);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		object->draw(*shader);
		shader->SetUniform("model", model);
		shader->SetUniform("view", camera->getView());
		shader->SetUniform("projection", camera->getProjection());
		// update other events like input handling
		glfwPollEvents();
		// put the stuff we’ve been drawing onto the display
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}