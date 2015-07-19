#include "ContactListener.h"
#include "GameObject.h"
#include "Message.h"
#include <iostream>


void ContactListener::BeginContact(b2Contact* contact)
{

	b2Fixture* a = contact->GetFixtureA();
	b2Fixture* b = contact->GetFixtureB();

	int typeA = *(int*)a->GetUserData();
	int typeB = *(int*)b->GetUserData();

	GameObject* objectA = static_cast<GameObject*>(a->GetBody()->GetUserData());
	GameObject* objectB = static_cast<GameObject*>(b->GetBody()->GetUserData());

	
	b2WorldManifold manifold;
	contact->GetWorldManifold(&manifold);


	if (objectA)
	{
		Message msgA("BEGIN_CONTACT", (void*)objectB, (void*)typeA, (void*)typeB, (void*)&manifold);
		objectA->sendMessage(msgA);
	}

	if (objectB)
	{
		Message msgB("BEGIN_CONTACT", (void*)objectA, (void*)typeB, (void*)typeA, (void*)&manifold);
		objectB->sendMessage(msgB);
	}

}

void ContactListener::EndContact(b2Contact* contact)
{


	b2Fixture* a = contact->GetFixtureA();
	b2Fixture* b = contact->GetFixtureB();


	int typeA;
	int typeB;
	if (a->GetUserData())
		typeA = *(int*)a->GetUserData();
	else
		typeA = 0;
	if (b->GetUserData())
		typeB = *(int*)b->GetUserData();
	else
		typeB = 0;

	GameObject* objectA = (GameObject*) a->GetBody()->GetUserData();
	GameObject* objectB = (GameObject*) b->GetBody()->GetUserData();

	
	b2WorldManifold manifold;
	contact->GetWorldManifold(&manifold);

	
	if (objectA)
	{
		Message msgA("END_CONTACT", (void*)objectB, (void*)typeA, (void*)typeB, (void*)&manifold);
		objectA->sendMessage(msgA);
	}

	if (objectB)
	{
		Message msgB("END_CONTACT", (void*)objectA, (void*)typeB, (void*)typeA, (void*)&manifold);
		objectB->sendMessage(msgB);
	}


}




void ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
{


	b2Fixture* a = contact->GetFixtureA();
	b2Fixture* b = contact->GetFixtureB();

	FixtureType typeA = *(FixtureType*)a->GetUserData();
	FixtureType typeB = *(FixtureType*)b->GetUserData();

	GameObject* objectA = (GameObject*) a->GetBody()->GetUserData();
	GameObject* objectB = (GameObject*) b->GetBody()->GetUserData();
	
	b2WorldManifold worldManifold;

	contact->GetWorldManifold(&worldManifold);

	worldManifold.points[0];

	if (typeA & PLAYER && typeB & DEADLY)
	{
		Message msg("DIE", (void*)&impulse->normalImpulses[0], (void*) &worldManifold.points[0]);
		objectA->sendMessage(msg);

	}


	if (typeB & PLAYER && typeA & DEADLY)
	{
		Message msg("DIE", (void*)&impulse->normalImpulses[0],(void*) &worldManifold.points[0]);
		objectB->sendMessage(msg);

	}

	//if (typeA & GROUND)
	//{
	//	b2Vec2 counterImpulse = contact->GetManifold()->localNormal;
	//	counterImpulse *= impulse->normalImpulses[0];

	//	b->GetBody()->ApplyLinearImpulse(b2Vec2(0.0f,counterImpulse.y), contact->GetManifold()->points[0].,true);
	//}

	//if (typeB & GROUND)
	//{
	//	b2Vec2 counterImpulse = contact->GetManifold()->localNormal;
	//	counterImpulse *= -impulse->normalImpulses[0];

	//	a->GetBody()->ApplyLinearImpulse(b2Vec2(0.0f,counterImpulse.y), contact->GetManifold()->localPoint,true);
	//}


}


void ContactListener::bubbleContact(GameObject* bubble, GameObject* other, State type)
{
	Message msg("NEW_AFFECTED", other);
	bubble->sendMessage(msg);
	other->setNextState(type);

}

