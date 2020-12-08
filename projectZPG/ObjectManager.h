#pragma once
#include "Object.h"
#include <vector>
class ObjectManager
{
private:
	std::vector<Object*> objectList;
public:
	void add(Object* object) {
		objectList.push_back(object);
	}

	std::vector<Object*> getObjects(){
		return objectList;
	}
};

