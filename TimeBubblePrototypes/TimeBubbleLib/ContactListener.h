#ifndef CONTACT_LISTENER_H
#define CONTACT_LISTENER_H

#include <Box2D\Box2D.h>
#include <string>
#include "GameObject.h"

class ContactListener : public b2ContactListener
{
public:
	
	void BeginContact(b2Contact* contact);
	void EndContact(b2Contact* contact);

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	//void setState(State _state);

private:
	//State state;

	void bubbleContact(GameObject* bubble, GameObject* other, State type);

	//void resolveContactBegin(b2Fixture* A, b2Fixture* B);
	//void resolveContactEnd(b2Fixture* A, b2Fixture* B);

	//void footCollision(b2Fixture* A,bool begin);

	//void timeBubbleBegin(b2Fixture* B, State type);
	//void timeBubbleEnd(b2Fixture* B);


};

#endif