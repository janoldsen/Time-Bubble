#include "EventComponent.h"
#include "Prototype.h"
#include <iostream>


EventComponent::EventComponent()
{
	for(int i= 0; i < MAX_EVENT; i++)
	{
		setMessage(i, "", 0.0f,0.0f,0.0f,0.0f);

	}
	
}

void EventComponent::write(std::fstream& stream)
{


	for(int i = 0; i < MAX_EVENT; i++)
	{
		stream.write((char*)&event[i].activation, sizeof(event[i].activation));
		stream.write((char*)&event[i].activator, sizeof(event[i].activator));
		
		int size = event[i].receiverName.size();
		stream.write((char*)&size, sizeof(size));
		stream.write(event[i].receiverName.c_str(), size * sizeof(char));

		size = event[i].msg.getName().size();
		stream.write((char*)&size, sizeof(size));
		stream.write(event[i].msg.getName().c_str(), size * sizeof(char));

		stream.write((char*)&event[i].arg0, sizeof(event[i].arg0));
		stream.write((char*)&event[i].arg1, sizeof(event[i].arg1));
		stream.write((char*)&event[i].arg2, sizeof(event[i].arg2));
		stream.write((char*)&event[i].arg3, sizeof(event[i].arg3));
	}

}


void EventComponent::read(std::fstream& stream, int versionNumber)
{

	int max;

	if (versionNumber < 7)
		max = 4;
	else
		max = MAX_EVENT;

	for(int i = 0; i < max; i++)
	{
		stream.read((char*)&event[i].activation, sizeof(event[i].activation));
		stream.read((char*)&event[i].activator, sizeof(event[i].activator));
		
		int size;
		stream.read((char*)&size, sizeof(size));
		event[i].receiverName = std::string(size, '\0');
		stream.read(&event[i].receiverName[0], size * sizeof(char));

		stream.read((char*)&size, sizeof(size));
		std::string str(size, '\0');
		stream.read(&str[0], size * sizeof(char));

		

		stream.read((char*)&event[i].arg0, sizeof(event[i].arg0));
		stream.read((char*)&event[i].arg1, sizeof(event[i].arg1));
		stream.read((char*)&event[i].arg2, sizeof(event[i].arg2));
		stream.read((char*)&event[i].arg3, sizeof(event[i].arg3));

		event[i].msg = Message(str, &event[i].arg0, &event[i].arg1, &event[i].arg2, &event[i].arg3);
	}
}


ComponentTypes EventComponent::getType()
{
	return ComponentTypes::EVENT;
}

AiComponent* EventComponent::clone()
{
	return new EventComponent(*this);
}


void EventComponent::initialize(GameObject* _parent)
{
	AiComponent::initialize(_parent);

	//event.activation = Event::ONBEGIN;
	//event.activator	 = FixtureType::PLAYER;
	//event.receiver	 = getGame()->findObject("Player");
	//event.msg		 = Message("DIE");



	for (int i = 0; i < MAX_EVENT; i++)
	{
		if (event[i].activation != NONE)
			event[i].receiver = getGame()->findObject(event[i].receiverName);
	}



}


void EventComponent::sendMessage(Message& msg)
{
	if (msg == "BEGIN_CONTACT")
	{
		for (int i = 0; i < MAX_EVENT; i++)
		{
			if (event[i].activation == Event::ONBEGIN && 
				(event[i].activator == 0 || (int)msg.getArgument(2) & event[i].activator))
			{
				if (event[i].receiver)
					event[i].receiver->sendMessage(event[i].msg);
				std::cout << parent->getName() << " to " << event[i].receiverName << ": " << event[i].msg.getName() << std::endl;
			}
		}
	}

	if (msg == "END_CONTACT")
	{
		for (int i = 0; i < MAX_EVENT; i++)
		{
			if (event[i].activation == Event::ONEND && 
				(event[i].activator == 0 || (int)msg.getArgument(2) & event[i].activator))
			{
				if (event[i].receiver)
					event[i].receiver->sendMessage(event[i].msg);
			std::cout << parent->getName() << " to " << event[i].receiverName << ": " << event[i].msg.getName() << std::endl;
			}
		}
	}


}



void EventComponent::setActivation(int id, Event::e activation)
{
	if (id >= MAX_EVENT)
		return;

	event[id].activation = activation;
}

void EventComponent::setActivator(int id, FixtureType activator)
{
	if (id >= MAX_EVENT)
		return;

	event[id].activator = activator;
}

void EventComponent::setReceiver(int id, std::string receiver)
{
	if (id >= MAX_EVENT)
		return;

	event[id].receiverName= receiver;
}

void EventComponent::setMessage(int id, std::string message, float _arg0, float _arg1, float _arg2, float _arg3)
{
	if (id >= MAX_EVENT)
		return;

	event[id].arg0 = _arg0;
	event[id].arg1 = _arg1;
	event[id].arg2 = _arg2;
	event[id].arg3 = _arg3;

	event[id].msg = Message(message,
		(void*)&event[id].arg0,
		(void*)&event[id].arg1,
		(void*)&event[id].arg2,
		(void*)&event[id].arg3);

}

void EventComponent::setMessage(int id, Message msg)
{
	if (id >= MAX_EVENT)
		return;

	event[id].msg = msg;

}



Event::e	 EventComponent::getActivation(int id)
{
	return event[id].activation;
}

int	 EventComponent::getActivator(int id)
{
	return event[id].activator;
}

std::string	 EventComponent::getReceiver(int id)
{
	return event[id].receiverName;
}

Message		 EventComponent::getMessage(int id)
{
	return event[id].msg;
}