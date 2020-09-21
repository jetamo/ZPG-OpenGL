//Include GLFW  


#include "GL/glew.h"
#include <GLFW/glfw3.h>  
#include <stdio.h>
#include "Application.h"

//Include GLM  
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

//Include the standard C++ headers  
#include <stdlib.h>
#include <stdio.h>


float points[] = {
	0.0f, 0.5f, 0.0f,
	0.5f, -0.5f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

const char* vertex_shader =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"void main () {"
"     gl_Position = vec4 (vp, 1.0);"
"}";

const char* fragment_shader =
"#version 330\n"
"out vec4 frag_colour;"
"void main () {"
"     frag_colour = vec4 (0.5, 0.0, 0.5, 1.0);"
"}";


static double mouseX = 0;
static double mouseY = 0;

Application* app = new Application();

static void error_callback(int error, const char* description) { fputs(description, stderr); }

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	printf("key_callback [%d,%d,%d,%d] \n", key, scancode, action, mods);
}

static void window_focus_callback(GLFWwindow* window, int focused) { printf("window_focus_callback \n"); }

static void window_iconify_callback(GLFWwindow* window, int iconified) { printf("window_iconify_callback \n"); }

static void window_size_callback(GLFWwindow* window, int width, int height) {
	printf("resize %d, %d \n", width, height);
	glViewport(0, 0, width, height);
}

//static void cursor_callback(GLFWwindow* window, double x, double y) { printf("%f, %f \n", mouseX, mouseY); }

//static void cursor_callback(GLFWwindow* window, double mouseXPos, double mouseYPos) {
//	app->cursor_pos_callback(window, mouseXPos, mouseYPos);
//}


//glfwSetCursorPosCallback(window, [](GLFWwindow* window, double mouseXPos, double mouseYPos)
//	-> void {Application::getInstance()->; cursor_pos_callback(window, mouseXPos, mouseYPos); });


static void button_callback(GLFWwindow* window, int button, int action, int mode) {
	if (action == GLFW_PRESS) printf("button_callback [%d,%d,%d]\n", button, action, mode);
}



//GLM test

// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);

// Camera matrix
glm::mat4 View = glm::lookAt(
	glm::vec3(10, 10, 10), // Camera is at (4,3,-3), in World Space
	glm::vec3(0, 0, 0), // and looks at the origin
	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
);
// Model matrix : an identity matrix (model will be at the origin)
glm::mat4 Model = glm::mat4(1.0f);

int main(void)
{
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
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

	//vertex buffer object (VBO)
	GLuint VBO = 0;
	glGenBuffers(1, &VBO); // generate the VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points,
		GL_STATIC_DRAW);
	//vertex attribute object(VAO)
	GLuint VAO = 0;
	glGenVertexArrays(1, &VAO); //generate the VAO
	glBindVertexArray(VAO); //bind the VAO
	glEnableVertexAttribArray(0); //enable vertex attributes
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//create and compile shaders
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertex_shader, NULL);
	glCompileShader(vertexShader);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragment_shader, NULL);
	glCompileShader(fragmentShader);
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, fragmentShader);
	glAttachShader(shaderProgram, vertexShader);
	glLinkProgram(shaderProgram);

	GLint status;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(shaderProgram, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	//// Sets the key callback
	glfwSetKeyCallback(window, key_callback);
	
	//glfwSetCursorPosCallback(window, cursor_callback);
	//glfwSetCursorPosCallback(window, Application::);


	glfwSetMouseButtonCallback(window, button_callback);
	
	glfwSetWindowFocusCallback(window, window_focus_callback);
	
	glfwSetWindowIconifyCallback(window, window_iconify_callback);
	
	glfwSetWindowSizeCallback(window, window_size_callback);
	


	while (!glfwWindowShouldClose(window))
	{
		glfwGetCursorPos(window, &mouseX, &mouseY);
		// clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		// draw triangles
		glDrawArrays(GL_TRIANGLES, 0, 3); //mode,first,count
		// update other events like input handling
		glfwPollEvents();
		// put the stuff we’ve been drawing onto the display
		glfwSwapBuffers(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}


	//GLFWwindow* window;
	//glfwSetErrorCallback(error_callback);
	//
	//if (!glfwInit())
	//	exit(EXIT_FAILURE);
	//window = glfwCreateWindow(640, 480, "ZPG", NULL, NULL);
	//if (!window)
	//{
	//	glfwTerminate();
	//	exit(EXIT_FAILURE);
	//}
	//glfwMakeContextCurrent(window);
	//glfwSwapInterval(1);
	//
	//// Sets the key callback
	//glfwSetKeyCallback(window, key_callback);
	//
	//glfwSetCursorPosCallback(window, cursor_callback);
	//
	//glfwSetMouseButtonCallback(window, button_callback);
	//
	//glfwSetWindowFocusCallback(window, window_focus_callback);
	//
	//glfwSetWindowIconifyCallback(window, window_iconify_callback);
	//
	//glfwSetWindowSizeCallback(window, window_size_callback);
	//
	//
	//int width, height;
	//glfwGetFramebufferSize(window, &width, &height);
	//float ratio = width / (float)height;
	//glViewport(0, 0, width, height);
	//
	//
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
	//
	//
	//while (!glfwWindowShouldClose(window))
	//{
	//	glClear(GL_COLOR_BUFFER_BIT);
	//
	//	glMatrixMode(GL_MODELVIEW);
	//	glLoadIdentity();
	//	glRotatef((float)glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
	//
	//	glBegin(GL_TRIANGLES);
	//	glColor3f(0.f, 0.f, 1.f);
	//	glVertex3f(-0.5f, -0.5f, 0.f);
	//	glColor3f(1.f, 0.f, 0.f);
	//	glVertex3f(0.5f, -0.5f, 0.f);
	//	glColor3f(1.f, 0.f, 0.f);
	//	glVertex3f(0.5f, 0.5f, 0.f);
	//	glEnd();
	//	//glfwSwapBuffers(window);
	//
	//	glBegin(GL_TRIANGLES);
	//	glColor3f(1.f, 0.f, 0.f);
	//	glVertex3f(0.5f, 0.5f, 0.f);
	//	glColor3f(0.f, 0.f, 1.f);
	//	glVertex3f(-0.5f, 0.5f, 0.f);
	//	glColor3f(0.f, 0.f, 1.f);
	//	glVertex3f(-0.5f, -0.5f, 0.f);
	//	glEnd();
	//	glfwSwapBuffers(window);
	//
	//	glfwPollEvents();
	//}
	//glfwDestroyWindow(window);
	//glfwTerminate();
	//exit(EXIT_SUCCESS);