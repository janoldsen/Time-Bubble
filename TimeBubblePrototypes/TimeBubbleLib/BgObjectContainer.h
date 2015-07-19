#ifndef BG_OBJECT_CONTAINER_H
#define BG_OBJECT_CONTAINER_H

#include "BackGroundObject.h"
#include "ObjectIterator.h"

class BgObjectContainer
{
public:
	BgObjectContainer();
	void addObject(BackGroundObject* object);
	void deleteObject(BackGroundObject* object);

	void resortObject(BackGroundObject* object);

	void shutdown();

	BgOIterator* begin;
	BgOIterator* end;

private:
};

#endif