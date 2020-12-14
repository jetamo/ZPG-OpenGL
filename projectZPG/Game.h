#pragma once
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

#include "suzi_smooth.h"
#include "sphere.h"
#include "Window.h"
#include "ObjectManager.h"
#include "Texture.h"
#include "Scene.h"
#include "SceneManager.h"
#include "Game.h"
#include "objectPoints.h"
#include "ShaderLoader/ShaderLoader.h"

class Game
{	 
private:
	Window* window;


	double mouseX;
	double mouseY;

	double oldMouseX;
	double oldMouseY;


	Shader* shaderSky;
	Shader* shader;


	Texture* skyBoxTexture;
	Texture* americaTexture;


	TextureObject* skybox;
	TextureObject* plainO;

	ObjectManager* objectManager;

	Scene* testScene;

	Light* light;

	Camera* camera;

	Renderer* renderer;

	const char* vertexTexture = "./Shaders/vertexTexture.glsl";
	const char* fragmentPhong = "./Shaders/fragmentPhong.glsl";

public:
	Game(Window* _window) {
		window = _window;
		
		init();
	}

	void init() {

		mouseX = 0;
		mouseY = 0;
		oldMouseX = 0;
		oldMouseY = 0;


		shaderSky = new Shader(vertexTexture, fragmentPhong);
		shader = new Shader(vertexTexture, fragmentPhong);


		skyBoxTexture = new Texture("kostka.png");
		americaTexture = new Texture("test.png");


		skybox = new TextureObject(TcubePoints, sizeof(TcubePoints), shaderSky, 3);
		plainO = new TextureObject(plain, sizeof(plain), shader, 4);


		skybox->setTexture(skyBoxTexture);
		plainO->setTexture(americaTexture);


		objectManager = new ObjectManager();

		plainO->setPosition(glm::vec3(0.f, 0.f, 0.f));
		objectManager->add(plainO);
		testScene = new Scene(objectManager);

		light = new Light(glm::vec3(5.f, 5.f, 5.f), glm::vec3(0.1f, 1.f, 0.1f), 2, 12.5f);
		testScene->addLight(light);

		camera = new Camera();

		CallbacksHandler::setCamera(*camera);

		renderer = new Renderer(camera);

		camera->registerObserver(*shader);
		camera->registerObserver(*shaderSky);
	}


	void run() {

		//create and compile shaders
		//Object* cube = new Object(cubePoints, sizeof(cubePoints), shader, 1);
		//Object* suzi = new Object(suziSmooth, sizeof(suziSmooth), shader, 2);
		//Object* circle3d = new Object(sphere, sizeof(sphere), shader, 3);

		//přidání ID do stencil bufferu
		glEnable(GL_STENCIL_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		//0.385f, 0.647f, 0.812f

		glEnable(GL_DEPTH_TEST);



		while (!glfwWindowShouldClose(window->getWindow()))
		{
			int lightNr = 0;
			for (Light* light : testScene->getLights()) {
				light->setUniforms(shader, lightNr);
				lightNr++;
			}

			oldMouseX = mouseX;
			oldMouseY = mouseY;
			glfwGetCursorPos(window->getWindow(), &mouseX, &mouseY);
			// clear color and depth buffer
			glm::dvec2 delta = glm::dvec2(mouseX, mouseY) - glm::dvec2(oldMouseX, oldMouseY);
			camera->checkForMovement(window->getWindow());
			camera->changeViewAngle(delta.x, -delta.y);
			glClearStencil(0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			skybox->setPosition(camera->getPosition());
			glDisable(GL_DEPTH_TEST);
			renderer->draw(skybox);
			glEnable(GL_DEPTH_TEST);

			renderer->draw(testScene);
			if (Application::clicked)
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
				Object* tmpObject = new Object(sphere, sizeof(sphere), shader, objectManager->getObjects().at(objectManager->getObjects().size() - 1)->getId() + 1);
				tmpObject->setPosition(pos);
				tmpObject->scale(glm::vec3(0.1, 0.1, 0.1));
				objectManager->add(tmpObject);
				Application::clicked = false;
			}

			// update other events like input handling
			glfwPollEvents();
			// put the stuff we’ve been drawing onto the display
			glfwSwapBuffers(window->getWindow());
		}
	}
};

