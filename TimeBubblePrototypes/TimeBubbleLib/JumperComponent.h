#ifndef JUMPER_COMPONENT_H
#define JUMPER_COMPONENT_H

#include "Aicomponent.h"


class JumperComponent :	public AiComponent
{
public:

	void write(std::fstream& stream);
	void read(std::fstream& stream, int versionNumber);

	ComponentTypes getType();
	
	AiComponent* clone();

	void initialize(GameObject* _parent);
	void sendMessage(Message& msg);
	void update();

	void setMaxJumpHeight(float _maxJumpHeight);
	float getMaxJumpHeight();

private:
	
	int numFootContacts;
	bool jumping;

	float maxJumpHeight;
	b2Vec2 jumpImpulse;


};


#endif