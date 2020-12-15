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
		glStencilFunc(GL_ALWAYS, object->getId(), 0xFF);
		object->bind();
		object->shader->bind();
		object->shader->setUniform("model", object->transform);
		if(object->texture != nullptr)
			object->texture->bindTexture(object->shader->getID());
		//glDrawArrays(GL_TRIANGLES, 0, object->pointsSize); //mode,first,count
		glDrawElements(GL_TRIANGLES, object->count, GL_UNSIGNED_INT, nullptr);
	}

	void draw(ObjectManager* objectMng)
	{
		for (Object* object : objectMng->getObjects()) {
			draw(object);
		}
	}

	void draw(Scene* scene)
	{
		draw(scene->getObjectManager());
	}
};