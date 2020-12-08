#pragma once
#include "Camera.h"
#include "Object.h"
#include "ObjectManager.h"
#include "Scene.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class Renderer
{
private:
	Camera* cam;
public:
	Renderer(Camera* _cam) 
	{
		cam = _cam;
	}

	void draw(Object* object)
	{
		object->bind();
		object->shader->bind();
		object->shader->setUniform("model", object->transform);
		glDrawArrays(GL_TRIANGLES, 0, object->pointsSize); //mode,first,count
	}

	void draw(ObjectManager* objectMng)
	{
		for (Object* object : objectMng->getObjects()) {
			object->bind();
			object->shader->bind();
			object->shader->setUniform("model", object->transform);
			glDrawArrays(GL_TRIANGLES, 0, object->pointsSize); //mode,first,count
		}
	}

	void draw(Scene* scene)
	{
		for (Object* object : scene->getObjectManager()->getObjects()) {
			object->bind();
			object->shader->bind();
			object->shader->setUniform("model", object->transform);
			glDrawArrays(GL_TRIANGLES, 0, object->pointsSize); //mode,first,count
		}
	}
};