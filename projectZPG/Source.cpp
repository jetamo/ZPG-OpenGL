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

#include "suzi_smooth.h"
#include "sphere.h"
#include "plain.h"

const char* vertex_shader =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"layout(location=1) in vec3 vertNormal;"
"uniform mat4 model;"
"uniform mat4 view;"
"uniform mat4 projection;"
"uniform vec3 lightPosition;"
"uniform vec3 camPosition;"
"out vec3 ex_worldPosition;"
"out vec3 ex_worldNormal;"
"out vec3 ex_fragPos;"
"out vec3 ex_lightPosition;"
"out vec3 ex_camPosition;"
"void main () {"
"     gl_Position = (projection * view * model) * vec4(vp, 1.0f);"
"     mat3 normalMatrix = transpose(inverse(mat3(model)));"
"	  ex_worldPosition = vec3(model * vec4(vp, 1.0f));"
"	  ex_worldNormal = normalMatrix * vertNormal;"
"     ex_lightPosition = lightPosition;"
"	  ex_camPosition = camPosition;"
"}";

const char* fragment_shader =
"#version 330\n"
"in vec3 ex_worldPosition;"
"in vec3 ex_worldNormal;"
"in vec3 ex_vector;"
"in vec3 ex_lightPosition;"
"in vec3 ex_camPosition;"
"out vec4 frag_colour;"
"void main () {"
"     vec3 lightVector = ex_lightPosition - ex_worldPosition;"
"     lightVector = normalize(lightVector);"
"     vec3 viewDirection = normalize(ex_camPosition - ex_worldPosition);"
"     vec3 reflectionVector = reflect(-lightVector, ex_worldNormal);"
"     vec4 specular = (pow(max(dot(normalize(viewDirection), normalize(reflectionVector)), 0.0), 32)) * vec4(0.385, 0.647, 0.812, 1.0);"
"     float dot_product = max(dot(lightVector, normalize(ex_worldNormal)), 0.0);"
"     vec4 diffuse = dot_product * vec4(0.385, 0.647, 0.812, 1.0);"
"     vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);"
"     frag_colour = ambient + diffuse + specular;"
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

	const float cubePoints[] = { -0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, 1.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,

		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, };

	//create and compile shaders
	Shader* shader = new Shader(vertex_shader, fragment_shader);
	Object* cube = new Object(cubePoints, sizeof(cubePoints));
	Object* suzi = new Object(suziSmooth, sizeof(suzi));
	Object* circle3d = new Object(sphere, sizeof(sphere));
	Camera* camera = new Camera();

	Application::setCamera(*camera);

	camera->registerObserver(*shader);


	glm::mat4 model;
	float rot = 0.0f;

	glEnable(GL_DEPTH_TEST);

	shader->setUniform("lightPosition", glm::vec3(0.0f, 0.0f, 0.0f));

	while (!glfwWindowShouldClose(window))
	{
		

		oldMouseX = mouseX;
		oldMouseY = mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		// clear color and depth buffer
		glm::dvec2 delta = glm::dvec2(mouseX, mouseY) - glm::dvec2(oldMouseX, oldMouseY);
		camera->checkForMovement(window);
		camera->changeViewAngle(delta.x, -delta.y);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		model = glm::translate(glm::mat4(1.0f), glm::vec3(1.f, 0.f, 0.f));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		shader->setUniform("model", model);
		circle3d->draw(*shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.f, 0.f, 0.f));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		shader->setUniform("model", model);
		circle3d->draw(*shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, 1.f));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		shader->setUniform("model", model);
		circle3d->draw(*shader);

		model = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -1.f));
		model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		shader->setUniform("model", model);
		circle3d->draw(*shader);

		// update other events like input handling
		glfwPollEvents();
		// put the stuff we’ve been drawing onto the display
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}