#ifndef EVENT_COMPONENT_H
#define EVENT_COMPONENT_H

#include "AiComponent.h"

#define MAX_EVENT 8

struct Event
{

	Event() 
	{
		activation = NONE; activator = 0; receiver = 0;
		arg0 = 0; arg1 = 0; arg2 = 0; arg3 = 0;
#

	};

	enum e
	{
		NONE = 0,
		ONBEGIN = 1,
		ONEND = 2
	} activation;

	
	int activator;
	std::string receiverName;
	GameObject* receiver;
	
	Message msg;
	float arg0, arg1, arg2, arg3;


};

class EventComponent : public AiComponent
{
public:

	EventComponent();
	void write(std::fstream& stream);
	void read(std::fstream& stream, int versionNumber);

	ComponentTypes getType();

	AiComponent* clone();

	void initialize(GameObject* _parent);

	void setActivation(int id, Event::e activation);
	void setActivator(int id, FixtureType activator);
	void setReceiver(int id, std::string receiver);
	void setMessage(int id, Message msg);
	void EventComponent::setMessage(int id, std::string message, float _arg0 = 0.0f, float _arg1 = 0.0f, float _arg2 = 0.0f, float _arg3 = 0.0f);

	Event::e	 getActivation(int id);
	int	 getActivator(int id);
	std::string	 getReceiver(int id);
	Message		 getMessage(int id);


	void sendMessage(Message& msg);

private:

	Event event[MAX_EVENT];

};


#endif