#include "TimeBubbleComponent.h"
#include "GameObject.h"
#include "Message.h"
#include "GameClock.h"
#include "Prototype.h"
#include <iostream>
#include "myMath.h"


TimeBubbleComponent::TimeBubbleComponent()
{
	owner = NULL;
	lifeTime = 2.0f;
	active = false;
	radius = 2.0f;


}


void TimeBubbleComponent::initialize(GameObject* _parent)
{
	AiComponent::initialize(_parent);

	if (getBody())
	{

		b2CircleShape shape;
		shape.m_radius = 0.5f;

		b2FixtureDef fixtureDef;
		fixtureDef.isSensor = true;
		fixtureDef.shape = &shape;
		fixtureDef.density = 1.0f;
		fixtureDef.userData = new int(BUBBLE_INNER);

		parent->addFixture(&fixtureDef);

	}

	alive = true;

}

AiComponent* TimeBubbleComponent::clone()
{
	return new TimeBubbleComponent(*this);
}


void TimeBubbleComponent::write(std::fstream& stream)
{
	stream.write((char*)&lifeTime, sizeof(lifeTime));
	stream.write((char*)&type, sizeof(type));
	stream.write((char*)&radius, sizeof(radius));

}

void TimeBubbleComponent::read(std::fstream& stream, int versionNumber)
{
	stream.read((char*)&lifeTime, sizeof(lifeTime));
	stream.read((char*)&type, sizeof(type));

	if (versionNumber >= 13)
		stream.read((char*)&radius, sizeof(radius));

}


void TimeBubbleComponent::update()
{
	if (owner)
	{
		parent->setPosition(owner->getCenter());
		getBody()->SetLinearVelocity(owner->getBody()->GetLinearVelocity());
	}

	//else
	//{
	//	for (int i = 0; i < affected.size(); i++)
	//	{
	//		if (affected[i]->testPoint(MATH::sfTob2(parent->getPosition())))
	//		{
	//			owner = affected[i];
	//			setActive(true);
	//		}
	//	}
	//}


	if (active)
	{
		lifeTime -= gameClock->getElapsedTime();

		if (lifeTime <= 0.0f)
		{
			pop();
			getGame()->destroyObject(parent);
		}

	}


}


void TimeBubbleComponent::draw(Camera* camera, sf::RenderTarget* target)
{
	camera->addTimeBubble(this);
}

void TimeBubbleComponent::setOwner(GameObject* _owner)
{
	owner = _owner;
}
void TimeBubbleComponent::setPlayer(GameObject* _player)
{
	player = _player;
}



void TimeBubbleComponent::sendMessage(Message& msg)
{

	if (msg == "BEGIN_CONTACT")
	{
		int typeA = (int) msg.getArgument(1);
		int typeB = (int) msg.getArgument(2);

		if (typeA & BUBBLE && typeB & GROUND && getBody()->GetLinearVelocity().LengthSquared() >= 0.0f)
		{
			getBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
			getBody()->SetGravityScale(0.0f);
		}


		

		if (typeA & BUBBLE && typeB & DYNAMIC)
		{
			GameObject* object = static_cast<GameObject*>(msg.getArgument(0));
		
			if (alive)
			{

				bool alreadyAffected = false;
				for (int i = 0; i < affected.size(); i++)
				{
					if (affected[i] == object)
					{
						alreadyAffected = true;
						numFix[i]++;
						break;
					}

				}

				if (!alreadyAffected)
				{
					affected.push_back(object);
					numFix.push_back(1);
					object->setNextState(type);
				}
			}
		}
		if (typeA & BUBBLE && typeB & BUBBLE)
		{
			GameObject* other = static_cast<GameObject*>(msg.getArgument(0));

			if (!owner)
			{
				Message msg("POP");
				
				other->sendMessage(msg);
				getGame()->destroyObject(other);
				
				//pop();
				//getGame()->destroyObject(parent);
			}
		}

		if (typeA & BUBBLE_INNER && typeB & DYNAMIC)
		{
			if (!owner)
			{
				GameObject* object = static_cast<GameObject*>(msg.getArgument(0));

				owner = object;
				active = true;
			}

		}



	}

	if (msg == "END_CONTACT")
	{
		int typeA = (int)msg.getArgument(1);
		int typeB = (int)msg.getArgument(2);

		if (typeA & BUBBLE && typeB & DYNAMIC)
		{
			GameObject* object = static_cast<GameObject*>(msg.getArgument(0));

			bool lastContact = true;
			int i = 0;
			for (; i < affected.size(); i++)
			{
				if (object == affected[i])
				{
					lastContact = (--numFix[i] == 0);
					break;
				}
			}

			if (lastContact)
			{
				object->setNextState(NORMAL);
				
				affected.erase(affected.begin() + i);
				numFix.erase(numFix.begin() + i);

			}


			//bool lastContact = true;
			//int count = 0;
			//for (b2ContactEdge* ce = getBody()->GetContactList(); ce; ce = ce->next)
			//{
			//	b2Contact* c = ce->contact;

			//	if (object == (GameObject*)c->GetFixtureB()->GetBody()->GetUserData()
			//		|| object == (GameObject*)c->GetFixtureA()->GetBody()->GetUserData())
			//	{
			//		//lastContact = false;
			//		//break;
			//		count++;
			//		std::cout << "lskjdfl" << std::endl;
			//	}
			//}

			//if (count <= 1)
			//{
			//	object->setNextState(NORMAL);
			//	for (int i = 0; i < affected.size(); i++)
			//	{
			//		if (affected[i] == object)
			//		{
			//			affected.erase(affected.begin() + i);
			//			break;
			//		}
			//	}
			//}

		}


	}



	if (msg == "NEW_AFFECTED")
	{
		GameObject* object = static_cast<GameObject*>(msg.getArgument(0));
		

		bool alreadyAffected = false;
		for (int i = 0; i < affected.size(); i++)
		{
			if (affected[i] == object)
			{
				alreadyAffected = true;
				numFix[i]++;
				break;
			}

		}

		if (!alreadyAffected)
		{
			affected.push_back(object);
			numFix.push_back(1);
			object->setNextState(type);
		}
	}

	else if (msg == "PLAYER_DEAD")
	{


		for(int i = 0; i < affected.size(); i++)
		{
			if (affected[i] == player)
			{
				affected.erase(affected.begin() + i);
				numFix.erase(numFix.begin() + i);
				break;
			}
		}

		player = 0;
	}

	else if (msg == "POP")
	{
		pop();
	}

}

void TimeBubbleComponent::shutdown()
{
	if (player)
	{
		Message msg("BUBBLE_DESTROYED", parent);
		player->sendMessage(msg);
	}
}


void TimeBubbleComponent::pop()
{
	std::cout << "blah	" << type << std::endl;

	for (int i = 0; i < affected.size(); i++)
	{
		affected[i]->setNextState(NORMAL);
	}

	alive = false;

}

ComponentTypes TimeBubbleComponent::getType()
{
	return ComponentTypes::BUBBLE;
}

void TimeBubbleComponent::setType(State _type)
{
	type = _type;
}



void TimeBubbleComponent::setLifeTime(float _lifeTime)
{
	lifeTime = _lifeTime;
}

float TimeBubbleComponent::getLifeTime()
{
	return lifeTime;
}

void TimeBubbleComponent::setActive(bool _active)
{
	active = _active;
}




void TimeBubbleComponent::setRadius(float _radius)
{
	radius = _radius;
}

float TimeBubbleComponent::getRadius() const
{
	return radius;
}