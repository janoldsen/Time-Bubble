#include "ObjectContainer.h"
#include <iostream>

ObjectContainer::ObjectContainer()
{
	begin = 0;
	end = 0;
}



void ObjectContainer::addObject(GameObject* object, bool front)
{

	GOIterator* it = new GOIterator();
	it->object = object;



	if (!begin && !end)
	{
		begin = it;
		end = it;
		return;
	}

	if (front)
	{
		it->next = begin;
		begin->previous = it;
		begin = it;
	}
	else
	{
		it->previous = end;
		end->next = it;
		end = it;
	}





}
void ObjectContainer::deleteObject(GameObject* object, bool startFromFront)
{
	GOIterator* it;
	if (startFromFront)
		it = begin;
	else
		it = end;
	while (it)
	{
		if (it->object == object)
		{
			it->destroyed = true;
			return;
		}
		if (startFromFront)
			it = it->next;
		else
			it = it->previous;

	}

}

void ObjectContainer::clean()
{
	//ObjectIterator* previous = NULL;
	GOIterator* next = NULL;

	for (GOIterator* it = begin; it; it = next)
	{
		next = it->next;

		if (it->destroyed)
		{

			if (it->previous)
				it->previous->next = it->next;
			else
				begin = it->next;

			if (next)
				next->previous = it->previous;

			if (it == end)
				end = it->previous;

			it->object->shutdown();
			delete it->object;
			it->object = NULL;
			delete it;
		}
		//else
		//{  
		//	previous = it;
		//}


	}
}


void ObjectContainer::shutdown()
{
	GOIterator* previous;
	for (GOIterator* it = end; it; it = previous)
	{
		previous = it->previous;
		it->object->shutdown();
		delete it->object;
		delete it;
	}

	begin = 0;
	end = 0;

}



GameObject* ObjectContainer::find(std::string name)
{
	for (GOIterator* it = begin; it; it = it->next)
	{
		if (it->object->getName() == name)
			return it->object;
	}

	return 0;
}