#include "BgObjectContainer.h"
#include <iostream>

BgObjectContainer::BgObjectContainer()
{
	begin = 0;
	end = 0;
}


void BgObjectContainer::addObject(BackGroundObject* object)
{
	BgOIterator* it = new BgOIterator();
	it->object = object;



	if (!begin && !end)
	{
		begin = it;
		end = it;
	}
	else if (begin->object->getDepth() <= object->getDepth())
	{
		it->next = begin;
		begin->previous = it;
		begin = it;
	}
	else if (end->object->getDepth() > object->getDepth())
	{
		it->previous = end;
		end->next = it;
		end = it;
	}
	else
	{
		BgOIterator* it2 = begin->next;
		for (;it2; it2 = it2->next)
		{
			if (it2->object->getDepth() <= object->getDepth())
			{
				it2->previous->next = it;
				it->previous = it2->previous;
				
				it2->previous = it;
				it->next = it2;
				
				return;
			}
		}

	}



}


void BgObjectContainer::deleteObject(BackGroundObject* object)
{
	BgOIterator* it = begin;
	BgOIterator* next; 


	for (; it; it = next)
	{
		next = it->next;

		if (it->object == object)
		{
			
			
			if (it->previous)
				it->previous->next = it->next;
			else
				begin = it->next;

			if (next)
				next->previous = it->previous;

			if (it == end)
				end = it->previous;



			//if (it == begin)
			//{
			//	begin = it->next;
			//	it->next->previous = 0;
			//}
			//else if(it == end)
			//{
			//	end = it->previous;
			//	it->previous->next = 0;
			//}
			//else
			//{
			//	it->previous->next = it->next;
			//	it->next->previous = it->previous;
			//}


			delete it->object;
			delete it;
			return;
		}

	}
}


void BgObjectContainer::shutdown()
{
	BgOIterator* it = begin;
	BgOIterator* next;

	for (; it; it = next)
	{
		next = it->next;

		delete it->object;
		delete it;

	}

	begin = 0;
	end = 0;

}


void BgObjectContainer::resortObject(BackGroundObject* object)
{
	BgOIterator* objectIt = 0;
	BgOIterator* newIt = 0;


	




	BgOIterator* it = begin;
	BgOIterator* _front =  0;
	for (; it; it = it->next)
	{



		if (it->object == object)
		{
			objectIt = it;
		}
		else if( it->object->getDepth() <= object->getDepth() && !newIt)
		{
			newIt = it;
		}
	}


	if (!objectIt)
	{
		std::cout << "BackGroundObject not found" << std::endl;
		return;
	}






	if(objectIt == begin && objectIt == end)
		return;

	if (objectIt == begin)
	{
		begin = objectIt->next;
		objectIt->next->previous = 0;
	}
	else if(objectIt == end)
	{
		end = objectIt->previous;
		objectIt->previous->next = 0;
	}
	else
	{
		objectIt->previous->next = objectIt->next;
		objectIt->next->previous = objectIt->previous;
	}


	if (newIt == begin)
	{
		begin = objectIt;
		newIt->previous = objectIt;
		
		objectIt->next = newIt;
		objectIt->previous = 0;
	}
	else if(!newIt)
	{
		
		end->next = objectIt;

		objectIt->next = 0;
		objectIt->previous = end;

		end = objectIt;
	}
	else
	{
		newIt->previous->next = objectIt;
		objectIt->previous = newIt->previous;
		
		newIt->previous = objectIt;
		objectIt->next = newIt;
	}


}