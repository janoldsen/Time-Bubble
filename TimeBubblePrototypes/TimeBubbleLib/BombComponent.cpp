#include "BombComponent.h"
#include "GameClock.h"
#include "myMath.h"
#include "Prototype.h"

BombComponent::BombComponent()
{
	explosion = 0;
	explosionRadius = 10.0f;

	attackRange = 5.0f;
}

void BombComponent::write(std::fstream& stream)
{
	stream.write((char*)&explosionRadius, sizeof(explosionRadius));
	stream.write((char*)&attackRange, sizeof(attackRange));

	EnemyComponent::write(stream);

}

void BombComponent::read(std::fstream& stream, int versionNumber)
{
	stream.read((char*)&explosionRadius, sizeof(explosionRadius));
	stream.read((char*)&attackRange, sizeof(attackRange));

	EnemyComponent::read(stream, versionNumber);
}


ComponentTypes BombComponent::getType()
{
	return ComponentTypes::BOMB;
}

AiComponent* BombComponent::clone()
{
	return new BombComponent(*this);
}

void BombComponent::initialize(GameObject* _parent)
{
	EnemyComponent::initialize(_parent);


	b2FixtureDef explosionDef;
	b2CircleShape circleShape;
	circleShape.m_radius = explosionRadius;
	explosionDef.shape = &circleShape;
	explosionDef.userData = new int(0.0f);
	explosionDef.isSensor = true;

	explosion = getParent()->addFixture(&explosionDef);

	explosionTimer = 0.0f;

	exploding = NOT;
}


void BombComponent::update()
{
	if (explosionTimer > 0.0f)
	{
		explosionTimer -= gameClock->getElapsedTime() * parent->getTimescale();
		if(explosionTimer < (2.4f - 1.3f))
		{
			exploding = EXPLODED;
			getBody()->SetAwake(false);
		}
		//return;
	}
	else if (exploding == EXPLODED)
		parent->setPosition(sf::Vector2f(-10000.0f,0.0f));

	if (exploding == EXPLODED)
	{
		for (b2ContactEdge* ce = getBody()->GetContactList(); ce; ce = ce->next)
		{
			if (ce->other->GetUserData())
			{

				GameObject* other = (GameObject*)ce->other->GetUserData();

				float impulse =  100.0 / (MATH::magnitude(other->getCenter() - parent->getCenter()) * 3.0f);
				impulse *= ce->other->GetMass();

				b2Vec2 p = MATH::sfTob2(parent->getCenter());

				Message msg("DIE",(void*)&impulse, (void*)&p);
				other->sendMessage(msg);

				if (ce->other->GetType() == b2_dynamicBody)
				{
					//ce->other->ApplyForceToCenter(MATH::sfTob2(MATH::normalize(other->getCenter() - parent->getCenter()) * 100.0f * impulse * gameClock->getElapsedTime()), true);
					ce->other->ApplyLinearImpulse(MATH::sfTob2(MATH::normalize(other->getCenter() - parent->getCenter()) * impulse), ce->other->GetWorldCenter(), true);
				}

			}
		}
		//exploding = EXPLODED;

		getBody()->SetActive(false);

		//getGame()->destroyObject(parent);
	
	}




	if (MATH::magnitudeSquared(getGame()->getPlayer()->getPosition() - parent->getPosition()) <= (attackRange * attackRange) && exploding == NOT)
	{
		exploding = EXPLODING;
		explosionTimer = 2.4f;

	}

	
	if (exploding != EXPLODED && exploding != EXPLODING)
		EnemyComponent::update();
	

	if (exploding == EXPLODING)
		scml->playAnimation("explosion");
		



}


void BombComponent::setAttackRange(float _attackRange)
{
	attackRange = _attackRange;
}

float BombComponent::getAttackRange()
{
	return attackRange;
}



void BombComponent::setExplosionRadius(float _explosionRadius)
{
	explosionRadius = _explosionRadius;
}

float BombComponent::getExplosionRadius()
{
	return explosionRadius;
}