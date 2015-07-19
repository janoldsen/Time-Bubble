#ifndef MESSAGE_H
#define MESSAGE_H

#define MAX_MESSAGE_SIZE 4

#include <string>

class Message
{


public:
	Message() {};
	Message(std::string _msg, void* argument0 = 0, void* argument1 = 0, void* argument2 = 0, void* argument3 = 0);


	~Message();

	bool operator==(const std::string& rhs);


	std::string getName();
	void* getArgument(int id);

private:
	void* args[MAX_MESSAGE_SIZE];
	std::string msg;



};

#endif