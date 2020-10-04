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

#include "Shader.h"


float points[] = {
	 -0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
      0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	  0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 1.0f
};

const char* vertex_shader =
"#version 330\n"
"layout(location=0) in vec3 vp;"
"layout(location=1) in vec3 color;"
"uniform mat4 model;"
"out vec3 v_color;"
"void main () {"
"     gl_Position = model * vec4(vp, 1.0f);"
"	  v_color = color;"
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

Application* app = new Application();

//GLM test

// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.01f, 100.0f);

// Camera matrix
glm::mat4 View = glm::lookAt(
	glm::vec3(10, 10, 10), // Camera is at (4,3,-3), in World Space
	glm::vec3(0, 0, 0), // and looks at the origin
	glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
);

int main(void)
{
	GLFWwindow* window;
	if (!glfwInit()) {
		fprintf(stderr, "ERROR: could not start GLFW3\n");
		exit(EXIT_FAILURE);
	}

	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, NULL);

	glEnableVertexAttribArray(1); //enable vertex attributes
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float)*6, (void*)(3*sizeof(float)));


	//create and compile shaders
	Shader* shader = new Shader(vertex_shader, fragment_shader);


	glm::mat4 M = glm::mat4(1.0f);;
	float rot = 0.0f;

	while (!glfwWindowShouldClose(window))
	{
		// clear color and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		//M = glm::rotate(glm::mat4(1.0f), glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
		//M = glm::rotate(M, glm::radians(rot), glm::vec3(1.0f, 0.0f, 0.0f));
		//M = glm::translate(M, glm::vec3(0.0f, 0.0f, 0.0f));
		//M = glm::scale(M, glm::vec3(0.5f));
		
		M = glm::translate(glm::mat4(1.0f), { 0, 0, 0 })
			* glm::rotate(glm::mat4(1.0f), glm::radians(rot), { 0, 0, 1 })
			* glm::scale(glm::mat4(1.0f), { 1, 1, 1 });
		
		rot += 0.1f;
		glfwGetCursorPos(window, &mouseX, &mouseY);

		shader->Bind();
		shader->SetUniformMat4("model", M);
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