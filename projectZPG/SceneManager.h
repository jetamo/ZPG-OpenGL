#pragma once
#include "Scene.h"
#include <vector>
class SceneManager
{
private:
	std::vector<Scene*> scenes;
public:
	void addScene(Scene* scene) {
		scenes.push_back(scene);
	}

	std::vector<Scene*> getScenes() {
		return scenes;
	}
};

