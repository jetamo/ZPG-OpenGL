#pragma once
#include <list>
#include "Observer.h"

class Subject
{
protected:
	std::list<Observer *> observerCollection;
public:
	virtual void registerObserver(Observer& observer) {
		observerCollection.push_front(&observer);
	}
	
	//void unregisterObserver(Observer& observer) {
	//	observerCollection.remove(observer);
	//}

	virtual void notifyObservers() = 0;
};

