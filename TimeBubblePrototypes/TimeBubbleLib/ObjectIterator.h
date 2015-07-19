#ifndef OBJECT_ITERATOR
#define OBJECT_ITERATOR
#include "GameObject.h"
#include "BackGroundObject.h"


template<typename T>
class ObjectIterator
{
public:
	ObjectIterator()
	{
		object = NULL;
		next = NULL;
		previous = NULL;
		destroyed = false;

	}

	T* object;
	ObjectIterator* next;
	ObjectIterator* previous;

	bool destroyed;

private:



};



typedef ObjectIterator<GameObject> GOIterator;
typedef ObjectIterator<BackGroundObject> BgOIterator;

#endif