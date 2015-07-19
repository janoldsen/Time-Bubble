#include "Message.h"
#include <iostream>
#include "GameObject.h"

Message::Message(std::string _msg, void* argument0, void* argument1, void* argument2, void* argument3)
{
	msg = _msg;
	args[0] = argument0;
	args[1] = argument1;
	args[2] = argument2;
	args[3] = argument3;


}




Message::~Message()
{

}


bool Message::operator==(const std::string& rhs)
{
	return rhs == msg;
}

//template<class argumentType>
//argumentType* Message::getArgument(int id)
//{
//	return static_cast<argumentType*>(args[id]);
//}


std::string Message::getName()
{
	return msg;
}

void* Message::getArgument(int id)
{
	return args[id];
}