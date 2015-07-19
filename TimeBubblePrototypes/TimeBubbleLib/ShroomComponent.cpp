#include "ShroomComponent.h"
#include "Defines.h"
#include "GameClock.h"
#include "Prototype.h"
#include "myMath.h"
#include <iostream>

ShroomComponent::ShroomComponent(void)
{
	bounceHeight = 5.0f;
}


void ShroomComponent::write(std::fstream& stream)
{
	EnemyComponent::write(stream);

	stream.write((char*)&bounceHeight, sizeof(bounceHeight));
}

void ShroomComponent::read(std::fstream& stream, int versionNumber)
{
	EnemyComponent::read(stream, versionNumber);
	stream.read((char*)&bounceHeight, sizeof(bounceHeight));
}


ComponentTypes ShroomComponent::getType()
{
	return ComponentTypes::SHROOM;
}

AiComponent* ShroomComponent::clone()
{
	return new ShroomComponent(*this);
}


void ShroomComponent::sendMessage(Message& msg)
{

	if (msg == "BEGIN_CONTACT")
	{
		GameObject* object = (GameObject*) msg.getArgument(0);

		int typeA = (int)msg.getArgument(1);
		int typeB = (int)msg.getArgument(2);

		if (typeA & BOUNCE && object) 
		{
			b2Vec2 normal = ((b2WorldManifold*)msg.getArgument(3))->normal;
			if (abs(normal.y) > abs(normal.x))
			{
				sf::Vector2f impulse = sf::Vector2f(0.0f, (-object->getBody()->GetLinearVelocity().y + sqrt(2.0f * GRAVITY * bounceHeight)) * object->getBody()->GetMass());

				Message impulseMsg("ADD_IMPULSE", (void*)&impulse.x, (void*)&impulse.y);

				object->sendMessage(impulseMsg);

				if (bounceCd <= 0.0f)
				{
					bounceCd = 1.0f;

					if (scml)
						scml->playAnimation("bounce");
				}

			}

		}

	}


	EnemyComponent::sendMessage(msg);
}


void ShroomComponent::update()
{

	if (bounceCd > 0.0f)
	{
		bounceCd -= gameClock->getElapsedTime();
		return;
	}
	
	sf::Vector2f d = getGame()->getPlayer()->getPosition() - parent->getPosition();
	bool dir = (getGame()->getPlayer()->getPosition().x - 
		parent->getPosition().x) * (parent->getOrientation() - 2) > 0;


	if (attackCd > 0.0f)
	{
		attackCd -= gameClock->getElapsedTime();

		if (attackCd < 0.85f && attackCd > 0.65f)
		{
			if (MATH::magnitudeSquared(d) <= 6.0f && dir)
			{
				float imp = 0.0f;			
				b2Vec2 p = MATH::sfTob2(parent->getPosition());
				Message msg("DIE", (void*)&imp, (void*)&p);
				getGame()->getPlayer()->sendMessage(msg);
			}

			//std::cout << "attack" << std::endl;
		}

		return;
	}


	EnemyComponent::update();
	

	if (MATH::magnitudeSquared(d) <= 6.0f && dir )
	{
		attackCd = 1.0f;	
		if (scml)
			scml->playAnimation("attack");
	}
}



void ShroomComponent::setBounceHeight(float _bounceHeight)
{
	bounceHeight = _bounceHeight;
}

float ShroomComponent::getBounceHeight() const
{
	return bounceHeight;
}