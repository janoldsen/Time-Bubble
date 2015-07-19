#include "EnemyComponent.h"
#include "Defines.h"
#include "GameClock.h"
#include "Prototype.h"
#include <iostream>

EnemyComponent::EnemyComponent()
{
	jumpInterval = 0.0f;
	maxJumpHeight = 0.0f;
	turnInterval = 0.0f;
	walkSpeed = 0.0f;

}

void EnemyComponent::write(std::fstream& stream)
{
	stream.write((char*)&jumpInterval, sizeof(jumpInterval));
	stream.write((char*)&maxJumpHeight, sizeof(maxJumpHeight));
	stream.write((char*)&turnInterval, sizeof(jumpInterval));
	stream.write((char*)&walkSpeed, sizeof(walkSpeed));
	


}

void EnemyComponent::read(std::fstream& stream, int versionNumber)
{
	stream.read((char*)&jumpInterval, sizeof(jumpInterval));
	stream.read((char*)&maxJumpHeight, sizeof(maxJumpHeight));
	stream.read((char*)&turnInterval, sizeof(jumpInterval));
	stream.read((char*)&walkSpeed, sizeof(walkSpeed));
}



ComponentTypes EnemyComponent::getType()
{
	return ComponentTypes::ENEMY;
}

AiComponent* EnemyComponent::clone()
{
	return new EnemyComponent(*this);
}

void EnemyComponent::initialize(GameObject* _parent)
{
	AiComponent::initialize(_parent);

	numFootContacts = 0;
	jumping = false;

	jumpImpulse.y = sqrt(2.0f * GRAVITY * maxJumpHeight ) * getBody()->GetMass();
	jumpImpulse.x = 0;

	jumpTimer = jumpInterval;

	turnTimer = turnInterval; 

	scml = dynamic_cast<SCMLComponent*>(getRenderer());

}

void EnemyComponent::update()
{

	jumpTimer -= gameClock->getElapsedTime() * parent->getTimescale();
	turnTimer -= gameClock->getElapsedTime() * parent->getTimescale();



	if (jumpTimer <= 0.0f  && maxJumpHeight != 0.0f)
	{

		if (numFootContacts && !jumping && maxJumpHeight >= 0.0f)
		{

			b2Vec2 impulse = jumpImpulse;
			impulse *= parent->getTimescale();

			getBody()->ApplyLinearImpulse(impulse, getBody()->GetWorldCenter(),true);

			jumping = true;
			jumpTimer = jumpInterval;
		}



	}
	
	if (getBody()->GetLinearVelocity().y <= 0)
			jumping = false;


	if (turnTimer <= 0.0f)
	{

		parent->setOrientation(parent->getOrientation() == RIGHT ? LEFT : RIGHT);
		turnTimer = turnInterval;
	}


	bool walking = false;

	if (numFootContacts && walkSpeed != 0.0f)
	{
		float speed = (walkSpeed * parent->getTimescale() * -(2 - parent->getOrientation()));

		if (abs(getBody()->GetLinearVelocity().x) <= abs(speed))
		{
			getBody()->ApplyLinearImpulse(b2Vec2((abs(speed) - abs(getBody()->GetLinearVelocity().x)) * -(2 - parent->getOrientation()), 0.0f), getBody()->GetWorldCenter(), true); 
			walking = true;

		}

	}

	if (scml)
	{
		if (jumping)
		{
			scml->playAnimation("jump_up");
		}
		else if (walking)
		{
			scml->playAnimation("walk");
		}
		else
		{
			scml->playAnimation("idle");
		}
	}
}


void EnemyComponent::sendMessage(Message& msg)
{
	
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

	if (msg == "DIE")
	{
		/*getGame()->destroyObject(parent);*/
		//parent->setActive(false);

		getBody()->SetFixedRotation(false);

		for (b2Fixture* fix = getBody()->GetFixtureList(); fix; fix = fix->GetNext())
		{
			fix->SetSensor(true);
		}
	}


}



void EnemyComponent::setJumpInterval(float _jumpInterval)
{
	jumpInterval = _jumpInterval;
}

float EnemyComponent::getJumpInterval() const
{
	return jumpInterval;
}

void EnemyComponent::setMaxJumpHeight(float _maxJumpHeight)
{
	maxJumpHeight = _maxJumpHeight;
}

float EnemyComponent::getMaxJumpHeight() const
{
	return maxJumpHeight;
}

void EnemyComponent::setTurnInterval(float _turnInterval)
{
	turnInterval = _turnInterval;
}

float EnemyComponent::getTurnInterval() const
{
	return turnInterval;
}

void EnemyComponent::setWalkSpeed(float _walkSpeed)
{
	walkSpeed = _walkSpeed;
}

float EnemyComponent::getWalkSpeed() const
{
	return walkSpeed;
}