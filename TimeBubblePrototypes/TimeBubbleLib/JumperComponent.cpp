#include "JumperComponent.h"
#include "GameClock.h"
#include <iostream>
#include "GameObject.h"
#include "Defines.h"


void JumperComponent::write(std::fstream& stream)
{
	stream.write((char*)&maxJumpHeight, sizeof(maxJumpHeight));
}

void JumperComponent::read(std::fstream& stream,int versionNumber)
{

	stream.read((char*)&maxJumpHeight, sizeof(maxJumpHeight));
}

void JumperComponent::sendMessage(Message& msg)
{
	//if (msg == "IS_GROUNDED")
	//{
	//	numFootContacts++;
	//}
	//if (msg == "IS_NOT_GROUNDED")
	//	numFootContacts--;


	if (msg == "BEGIN_CONTACT")
	{
		int typeA = (int)msg.getArgument(1);
		int typeB = (int)msg.getArgument(2);

		if (typeB & GROUND && typeA & FOOT)
		{
			numFootContacts++;
		}

	}

	if (msg == "END_CONTACT")
	{
		int typeA = (int)msg.getArgument(1);
		int typeB = (int)msg.getArgument(2);

		if (typeB & GROUND && typeA & FOOT)
		{
			numFootContacts--;
		}

	
	}

}


ComponentTypes JumperComponent::getType()
{
	return ComponentTypes::JUMPER;
}


AiComponent* JumperComponent::clone()
{
	return new JumperComponent(*this);
}


void JumperComponent::initialize(GameObject * _parent)
{

	AiComponent::initialize(_parent);

	numFootContacts = 0;
	jumping = false;

	jumpImpulse.y = sqrt(2.0f * GRAVITY * maxJumpHeight ) * getBody()->GetMass();
	jumpImpulse.x = 0;

}



void JumperComponent::update()
{


	if (numFootContacts && !jumping)
	{

		b2Vec2 impulse = jumpImpulse;
		impulse *= parent->getTimescale();

		getBody()->ApplyLinearImpulse(impulse, getBody()->GetWorldCenter(),true);

		jumping = true;
	}

	if (getBody()->GetLinearVelocity().y <= 0)
		jumping = false;

}


void JumperComponent::setMaxJumpHeight(float _maxJumpHeight)
{
	maxJumpHeight = _maxJumpHeight;
}

float JumperComponent::getMaxJumpHeight()
{
	return maxJumpHeight;
}