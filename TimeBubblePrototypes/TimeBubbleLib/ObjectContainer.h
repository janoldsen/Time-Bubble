#ifndef OBJECT_CONTAINER_H
#define OBJECT_CONTAINER_H

#include "GameObject.h"
#include "ObjectIterator.h"

class ObjectContainer
{
public:
	ObjectContainer();
	void addObject(GameObject* object, bool front = false);
	void deleteObject(GameObject* object, bool startFromFront = false);

	GameObject* find(std::string name);

	virtual void shutdown();

	virtual void clean();

	
	GOIterator* begin;
	GOIterator* end;

	
};


#endif