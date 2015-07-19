#include "RockComponent.h"
#include "Defines.h"
#include "Prototype.h"
#include "myMath.h"
#include <iostream>
#include "GameClock.h"

RockComponent::RockComponent()
{
	attackRange = 20.0f;
	attacking = false;

	attackCounter = 0.0f;
	attackTimer = 0.0f;

	rageNumber = 3;

	deathCounter = 0;

	playerDead = false;
}

void RockComponent::write(std::fstream& stream)
{
	EnemyComponent::write(stream);

	stream.write((char*)&attackRange, sizeof(attackRange));
}

void RockComponent::read(std::fstream& stream, int versionNumber)
{
	EnemyComponent::read(stream, versionNumber);

	stream.read((char*)&attackRange, sizeof(attackRange));
}


ComponentTypes RockComponent::getType()
{
	return ComponentTypes::ROCK;
}



AiComponent* RockComponent::clone()
{
	return new RockComponent(*this);
}


void RockComponent::initialize(GameObject* _parent)
{
	EnemyComponent::initialize(_parent);
}



void RockComponent::update()
{

	if (playerDead)
	{
		scml->playAnimation("player_dead");
		return;
	}


	if (attackTimer > 0.0f)
		attackTimer-= gameClock->getElapsedTime();

	if (numFootContacts > 0 && attackTimer <= 0.0f && attacking)
		attacking = false;

	if (attacking)
		return;


	

	if (MATH::magnitudeSquared(getGame()->getPlayer()->getPosition() - parent->getPosition()) <= (attackRange * attackRange))
	{
		attack();
	}

	EnemyComponent::update();

	if (attacking)
	{
		if (attackCounter > rageNumber)
			scml->playAnimation("attack_rage");
		else
			scml->playAnimation("attack");
	}

}

void RockComponent::attack()
{


	if (attackCounter == rageNumber)
		getBody()->SetGravityScale(getBody()->GetGravityScale() * 2.0f);

	float y = sqrt(2.0f * GRAVITY * getBody()->GetGravityScale() * maxJumpHeight ) * getBody()->GetMass() / parent->getTimescale();

	float t = 2.0f * y / (GRAVITY * getBody()->GetGravityScale());
	t *= parent->getTimescale() * parent->getTimescale();

	float x = (parent->getPosition().x - (getGame()->getPlayer()->getCenter().x + getGame()->getPlayer()->getBody()->GetLinearVelocity().x * t * 0.75f)) / t;

	b2Vec2 impulse =  -getBody()->GetLinearVelocity() + b2Vec2(-x, y);
	impulse *= getBody()->GetMass() * parent->getTimescale();

	getBody()->ApplyLinearImpulse(impulse, getBody()->GetWorldCenter(), true);

	attacking = true;
	attackCounter++;
	attackTimer = t / parent->getTimescale();

}

void RockComponent::sendMessage(Message& msg)
{
	if (msg == "BEGIN_CONTACT")
	{
		int typeB = (int)msg.getArgument(2);

		if (typeB & PLAYER)
		{
			if (attackCounter >= rageNumber && deathCounter < 5)
			{
				numFootContacts++;
				deathCounter++;
			}
			else
				playerDead = true;
		}

	}

	if (msg == "END_CONTACT")
	{
		int typeB = (int)msg.getArgument(2);

		if (typeB & PLAYER)
		{
			if (attackCounter >= rageNumber)
			{
				numFootContacts--;
			}
		}

	}


	EnemyComponent::sendMessage(msg);
}

void RockComponent::setAttackRange(float _attackRange)
{
	attackRange = _attackRange;
}


float RockComponent::getAttackRange()
{
	return attackRange;
}



void RockComponent::setRageNumber(int _rageNumber)
{
	rageNumber = _rageNumber;
}


int RockComponent::getRageNumber()
{
	return rageNumber;
}