#pragma once
#include "ObjectManager.h"
#include "Light.h"
#include "Light.h"
class Scene
{
private:
	ObjectManager* objectManager;
	std::vector<Light*> lights;
public:
	Scene(ObjectManager* _objectManager) {
		objectManager = _objectManager;
	}

	void addLight(Light* light) {
		lights.push_back(light);
	}

	ObjectManager* getObjectManager() {
		return objectManager;
	}
};

