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
#include "Camera.h"
#include "Light.h"
#include "Renderer.h"
#include <vector>

#include "suzi_smooth.h"
#include "sphere.h"
#include "plain.h"

Application Application::instance;
bool Application::clicked = false;

void Application::Start()
{
	const char* vertex_shader =
		"#version 330\n"
		"layout(location=0) in vec3 vp;"
		"layout(location=1) in vec3 vertNormal;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"uniform vec3 lightPosition;"
		"uniform vec3 lightColor;"
		"uniform vec3 camPosition;"
		"out vec3 ex_worldPosition;"
		"out vec3 ex_worldNormal;"
		"out vec3 ex_fragPos;"
		"out vec3 ex_lightPosition;"
		"out vec3 ex_camPosition;"
		"out vec3 ex_lightColor;"
		"void main () {"
		"     gl_Position = (projection * view * model) * vec4(vp, 1.0f);"
		"     mat3 normalMatrix = transpose(inverse(mat3(model)));"
		"	  ex_worldPosition = vec3(model * vec4(vp, 1.0f));"
		"	  ex_worldNormal = normalMatrix * vertNormal;"
		"     ex_lightPosition = lightPosition;"
		"     ex_lightColor = lightColor;"
		"	  ex_camPosition = camPosition;"
		"}";

	const char* fragment_shader =
		"#version 330\n"
		"in vec3 ex_worldPosition;"
		"in vec3 ex_worldNormal;"
		"in vec3 ex_vector;"
		"in vec3 ex_lightPosition;"
		"in vec3 ex_lightColor;"
		"in vec3 ex_camPosition;"
		"out vec4 frag_colour;"
		"void main () {"
		"     vec3 lightVector = ex_lightPosition - ex_worldPosition;"
		"     lightVector = normalize(lightVector);"
		"     vec3 viewDirection = normalize(ex_camPosition - ex_worldPosition);"
		"     vec3 reflectionVector = reflect(-lightVector, ex_worldNormal);"
		"     vec4 specular = (pow(max(dot(normalize(viewDirection), normalize(reflectionVector)), 0.0), 32)) * vec4(0.385, 0.647, 0.812, 1.0);"
		"     float dot_product = max(dot(lightVector, normalize(ex_worldNormal)), 0.0);"
		"     vec4 diffuse = dot_product * vec4(ex_lightColor, 1.0);"
		"     vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0);"
		"     frag_colour = ambient + diffuse + specular;"
		"}";



	static double mouseX = 0;
	static double mouseY = 0;

	static double oldMouseX = 0;
	static double oldMouseY = 0;

	GLFWwindow* window;
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}


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
		-> void {CallbacksHandler::getInstance().cursor_pos_callback(window, mouseXPos, mouseYPos); });

	glfwSetErrorCallback([](int error, const char* description)
		-> void {CallbacksHandler::getInstance().error_callback(error, description); });

	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		-> void {CallbacksHandler::getInstance().key_callback(window, key, scancode, action, mods); });


	glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mode)
		-> void {CallbacksHandler::getInstance().button_callback(window, button, action, mode); });

	glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int focused)
		-> void {CallbacksHandler::getInstance().window_focus_callback(window, focused); });

	glfwSetWindowIconifyCallback(window, [](GLFWwindow* window, int iconified)
		-> void {CallbacksHandler::getInstance().window_iconify_callback(window, iconified); });

	glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int width, int height)
		-> void {CallbacksHandler::getInstance().window_size_callback(window, width, height); });

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
	Object* cube = new Object(cubePoints, sizeof(cubePoints), shader, 1);
	Object* suzi = new Object(suziSmooth, sizeof(suziSmooth), shader, 2);
	Object* circle3d = new Object(sphere, sizeof(sphere), shader, 3);

	//cube->setPosition(glm::vec3(-1.f, 0.f, 0.f));
	suzi->setPosition(glm::vec3(2.f, 0.f, 0.f));
	suzi->rotate(20, glm::vec3(0.f, 1.f, 0.f));
	circle3d->setPosition(glm::vec3(-2.f, 0.f, 0.f));

	std::vector<Object*> objects;
	//objects.push_back(cube);
	objects.push_back(suzi);
	objects.push_back(circle3d);

	//přidání ID do stencil bufferu
	glEnable(GL_STENCIL_TEST);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	Camera* camera = new Camera();
	Light* light = new Light(glm::vec3(500.f, 0.f, 0.f), glm::vec3(0.385f, 0.647f, 0.812f));
	Renderer* renderer = new Renderer(camera);

	//0.385f, 0.647f, 0.812f

	CallbacksHandler::setCamera(*camera);

	camera->registerObserver(*shader);


	glm::mat4 model;
	float rot = 0.0f;

	glEnable(GL_DEPTH_TEST);




	while (!glfwWindowShouldClose(window))
	{
		shader->setUniform("lightPosition", glm::vec3(20.0f, 20.f, 0.f));
		shader->setUniform("lightColor", glm::vec3(0.1f, 1.f, 0.1f));


		oldMouseX = mouseX;
		oldMouseY = mouseY;
		glfwGetCursorPos(window, &mouseX, &mouseY);
		// clear color and depth buffer
		glm::dvec2 delta = glm::dvec2(mouseX, mouseY) - glm::dvec2(oldMouseX, oldMouseY);
		camera->checkForMovement(window);
		camera->changeViewAngle(delta.x, -delta.y);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		for (Object* object : objects) {
			glStencilFunc(GL_ALWAYS, object->getId(), 0xFF);
			renderer->draw(object);
		}
		if (clicked)
		{
			//načtení ID a pozice ve světových souřadnicích
			GLbyte color[4];
			GLfloat depth;
			GLuint index;
			
			GLint x = (GLint)mouseX;
			GLint y = (GLint)mouseY;
			
			int newy = 600 - y - 10;
			
			glReadPixels(x, newy, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, &color);
			glReadPixels(x, newy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
			glReadPixels(x, newy, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);
			
			printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u \n", x, y, color[0], color[1], color[2], color[3], depth, index);
			
			//Application::getInstance()-&gt;getScene()-&gt;getRenderer()-&gt;setSelect(index-1);
			
			glm::vec3 screenX = glm::vec3(x, newy, depth);
			glm::mat4 view = camera->getView();
			glm::mat4 projection = camera->getProjection();
			glm::vec4 viewPort = glm::vec4(0, 0, 800, 600);
			glm::vec3 pos = glm::unProject(screenX, view, projection, viewPort);
			
			printf("unProject[%f, %f, %f] \n", pos.x, pos.y, pos.z);
			Object* tmpObject = new Object(sphere, sizeof(sphere), shader, objects.at(objects.size() - 1)->getId() + 1);
			tmpObject->setPosition(pos);
			objects.push_back(tmpObject);
			Application::clicked = false;
		}

		//model = glm::translate(glm::mat4(1.0f), glm::vec3(0.f, 0.f, -1.f));
		//model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
		//shader->setUniform("model", model);
		//circle3d->draw(*shader);

		// update other events like input handling
		glfwPollEvents();
		// put the stuff we’ve been drawing onto the display
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);

}
