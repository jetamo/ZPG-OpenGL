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
	const char* vertex_shader =
		"#version 330\n"
		"layout(location=0) in vec3 vp;"
		"layout(location=1) in vec3 vertNormal;"
		"layout(location=2) in vec2 in_uv;"
		"uniform mat4 model;"
		"uniform mat4 view;"
		"uniform mat4 projection;"
		"uniform vec3 camPosition;"
		"out vec3 ex_worldPosition;"
		"out vec3 ex_worldNormal;"
		"out vec3 ex_fragPos;"
		"out vec3 ex_camPosition;"
		"out vec2 uv;"
		"void main () {"
		"     gl_Position = (projection * view * model) * vec4(vp, 1.0f);"
		"     mat3 normalMatrix = transpose(inverse(mat3(model)));"
		"	  ex_worldPosition = vec3(model * vec4(vp, 1.0f));"
		"	  ex_worldNormal = normalMatrix * vertNormal;"
		"	  ex_camPosition = camPosition;"
		"	  uv = in_uv;"
		"}";


	const char* fragment_shader =
		"#version 330\n"
		"const int nrOfLights = 1;"
		"struct light"
		"{"
		"vec3 lightPos;"
		"vec3 lightColor;"
		"vec3 direction;"
		"float radius;"
		"int type;"
		"};"
		"uniform light lights[nrOfLights];"
		"uniform vec3 camDirection;"
		"uniform sampler2D textureUnitID;"
		"in vec3 ex_worldPosition;"
		"in vec3 ex_worldNormal;"
		"in vec3 ex_vector;"
		"in vec3 ex_camPosition;"
		"in vec2 uv;"
		"out vec4 frag_colour;"
		"void main () {"
		"     frag_colour = vec4(0.0, 0.0, 0.0, 0.0);"
		"	  for(int i = 0; i < nrOfLights; i++)"
		"{"
		"		if(lights[i].type == 1)"
		"		{"
		"			vec3 lightVector = lights[i].lightPos - ex_worldPosition;"
		"			lightVector = normalize(lightVector);"
		"			vec3 viewDirection = normalize(ex_camPosition - ex_worldPosition);"
		"			vec3 reflectionVector = reflect(-lightVector, ex_worldNormal);"
		"			vec4 specular = (pow(max(dot(normalize(viewDirection), normalize(reflectionVector)), 0.0), 32)) * vec4(0.385, 0.647, 0.812, 1.0);"
		"			float dot_product = max(dot(lightVector, normalize(ex_worldNormal)), 0.0);"
		"			vec4 diffuse = dot_product * vec4(lights[i].lightColor, 1.0);"
		"			frag_colour +=  diffuse + specular;"
		"		}"
		"		else if(lights[i].type == 2)"
		"		{"
		"			vec3 lightVector = ex_camPosition - ex_worldPosition;"
		"			lightVector = normalize(lightVector);"
		//"					vec3 lightVector = lights[i].lightPos - ex_worldPosition;"
		//"					lightVector = normalize(lightVector);"
		//		"			if(dot(lightVector, normalize(lights[i].lightPos - vec3(0, 0, 0))) > lights[i].radius)"		
		"			if(dot(lightVector, normalize(camDirection)) > 0.99f)"
		"			{"
		"				vec3 viewDirection = normalize(ex_camPosition - ex_worldPosition);"
		"				vec3 reflectionVector = reflect(-lightVector, ex_worldNormal);"
		"				vec4 specular = (pow(max(dot(normalize(viewDirection), normalize(reflectionVector)), 0.0), 32)) * vec4(0.385, 0.647, 0.812, 1.0);"
		"				float dot_product = max(dot(lightVector, normalize(ex_worldNormal)), 0.0);"
		"				vec4 diffuse = dot_product * vec4(lights[i].lightColor, 1.0) * texture(textureUnitID, uv);"
		"				frag_colour +=  diffuse + specular;"
		"			}"
		"			else"
		"				frag_colour = vec4(0.0, 0.0, 0.0, 1.0);"
		"		}"
		"		else if(lights[i].type == 3)"
		"		{"
		"			vec3 lightVector = ex_camPosition - ex_worldPosition; "
		"			lightVector = normalize(lightVector);"
		"			vec3 viewDirection = normalize(ex_camPosition - ex_worldPosition);"
		"			vec3 reflectionVector = reflect(-lightVector, ex_worldNormal);"
		"			vec4 specular = (pow(max(dot(normalize(viewDirection), normalize(reflectionVector)), 0.0), 32)) * vec4(0.385, 0.647, 0.812, 1.0);"
		"			float dot_product = max(dot(lightVector, normalize(ex_worldNormal)), 0.0);"
		"			vec4 diffuse = dot_product * vec4(lights[i].lightColor, 1.0);"
		"			frag_colour +=  diffuse + specular;"
		"		}"
		"}"

		"vec4 ambient = vec4(0.1, 0.1, 0.1, 1.0) * texture(textureUnitID, uv); "
		"frag_colour += ambient;"
		//"frag_colour = texture(textureUnitID, uv);"
		//"     frag_colour += vec4(0.1, 0.0, 0.0, 1.0);"
		"}";



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

	Game* game = new Game(window, vertex_shader, fragment_shader);

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


