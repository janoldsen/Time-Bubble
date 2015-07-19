#ifndef PROPERTY_BOX_AI_H
#define PROPERTY_BOX_AI_H


#include "PropertyBox.h"
#include <PlattformComponent.h>
#include <SwingComponent.h>
#include <TimeBubbleComponent.h>
#include <JumperComponent.h>
#include <PlayerComponent.h>
#include <EventComponent.h>
#include <EnemyComponent.h>
#include <ShroomComponent.h>
#include <ParticleComponent.h>
#include <RockComponent.h>
#include <BombComponent.h>

class LevelObject;

class PropertyBoxAi : public PropertyBox
{
public:
	virtual ~PropertyBoxAi() {};

	virtual void build();
};




class PropertyBoxPlattform : public PropertyBoxAi
{
public:
	void setObject(LevelObject* _object);
	void setPlattformComponent(PlattformComponent* _ai);

	void build();
	void update();

private:

	class WayPointBox : public PropertyBox
	{
	public:
		void setPlattformComponent(PlattformComponent* _ai)
		{
			ai = _ai;
		}
		void build()
		{
			box = sfg::Box::Create();
			box->Pack(sfg::Label::Create("Waypoint:"));
			x = sfg::Entry::Create();
			y = sfg::Entry::Create();
			x->GetSignal(sfg::Entry::OnKeyPress).Connect(&WayPointBox::setWayPoint, this);
			y->GetSignal(sfg::Entry::OnKeyPress).Connect(&WayPointBox::setWayPoint, this);
			box->Pack(x);
			box->Pack(y);

		}
		void update()
		{
			if (!x->HasFocus() && !y->HasFocus())
			{
				std::stringstream ss;
				ss << ai->getWayPoint(id).x;
				x->SetText(ss.str());
				ss.str("");
				ss.clear();
				ss << ai->getWayPoint(id).y;
				y->SetText(ss.str());
				
			}
		}

		int id;
	private:

		PlattformComponent* ai;

		sfg::Entry::Ptr x;
		sfg::Entry::Ptr y;

		void setWayPoint()
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				sf::Vector2f wayPoint;
				std::stringstream ss;
				ss.str(x->GetText());
				ss >> wayPoint.x;
				ss.clear();
				ss.str(y->GetText());
				ss >> wayPoint.y;
				ai->setWayPoint(id, wayPoint);

				box->GrabFocus();

			}

		}

	};


	PlattformComponent* ai;
	LevelObject* object;

	sfg::Entry::Ptr startValue;
	//sfg::Entry::Ptr endpositionX;
	//sfg::Entry::Ptr endpositionY;
	sfg::Entry::Ptr velocity;
	sfg::Button::Ptr addWayPoint;
	sfg::Button::Ptr deleteWayPoint;
	std::vector<WayPointBox> wayPoints;

	void setStartValue();
	//void setEndPosition();
	void setVelocity();
	void doAddWayPoint();
	void doDeleteWayPoint();


};


class PropertyBoxSwing : public PropertyBoxAi
{
public:
	void setSwingComponent(SwingComponent* _ai);

	void build();
	void update();

private:
	SwingComponent* ai;
	
	sfg::Entry::Ptr startAngle;
	sfg::Entry::Ptr maxAngle;
	sfg::Entry::Ptr velocity;

	void setStartAngle();
	void setMaxAngle();
	void setVelocity();
};



class PropertyBoxBubble : public PropertyBoxAi
{
public:
	void setBubbleComponent(TimeBubbleComponent* _ai);

	void build();
	void update();

private:
	TimeBubbleComponent* ai;

	sfg::Entry::Ptr lifeTime;

	void setLifeTime();

};




class PropertyBoxJumper : public PropertyBoxAi
{
public:
	void setObject(LevelObject* _object);
	void setJumperComponent(JumperComponent* _ai);

	void build();
	void update();

private:
	JumperComponent* ai;
	LevelObject* object;

	sfg::Entry::Ptr maxJumpHeight;

	void setMaxJumpHeight();
};



class PropertyBoxPlayer : public PropertyBoxAi
{
public:

	void setPlayerComponent(PlayerComponent* _ai);
	void build();
	void update();

private:
	PlayerComponent* ai;

	sfg::Entry::Ptr numFastBubbles;
	sfg::Entry::Ptr numSlowBubbles;

	sfg::ComboBox::Ptr weapon;

	void setNumBubbles();
	void setWeapon();

};



class PropertyBoxEvent : public PropertyBoxAi
{
public:

	void setEventComponent(EventComponent* _ai);

	void build();
	void update();

private:
	
	EventComponent* ai;

	struct EventBox
	{
		int id;
		EventComponent* ai;



		sfg::ComboBox::Ptr activation;
		sfg::Entry::Ptr activator;
		sfg::Entry::Ptr receiver;
		sfg::Entry::Ptr message;

		sfg::Entry::Ptr arg0;
		sfg::Entry::Ptr arg1;
		sfg::Entry::Ptr arg2;
		sfg::Entry::Ptr arg3;


		void setActivation()
		{
			ai->setActivation(id, (Event::e)activation->GetSelectedItem());
		}
		void setActivator()
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{
				int _activator;
				std::stringstream ss(activator->GetText());
				ss >> _activator;

				ai->setActivator(id, (FixtureType)_activator);

				activation->GrabFocus();
			}
		}
		void setReceiver()
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{

				ai->setReceiver(id, receiver->GetText());

				activation->GrabFocus();
			}
		}
		void setMessage()
		{	
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
			{

				std::stringstream ss;

				ss.str(arg0->GetText());
				float _arg0;
				ss >> _arg0;
				ss.clear();

				ss.str(arg1->GetText());
				float _arg1;
				ss >> _arg1;
				ss.clear();

				ss.str(arg2->GetText());
				float _arg2;
				ss >> _arg2;
				ss.clear();

				ss.str(arg3->GetText());
				float _arg3;
				ss >> _arg3;
				ss.clear();



				//Message msg(message->GetText(), &_arg0, &_arg1, &_arg2);
				//ai->setMessage(id,msg);
				ai->setMessage(id, message->GetText(), _arg0, _arg1, _arg2, _arg3);

				activation->GrabFocus();
			}
		}

	} events[MAX_EVENT];


};



class PropertyBoxEnemy : public PropertyBoxAi
{
public:
	void setObject(LevelObject* _object);
	void setEnemyComponent(EnemyComponent* _ai);

	void build();
	void update();

private:
	EnemyComponent* ai;

	sfg::Entry::Ptr maxJumpHeight;
	sfg::Entry::Ptr	jumpInterval;
	sfg::Entry::Ptr walkSpeed;
	sfg::Entry::Ptr turnInterval;

	void setMaxJumpHeight();
	void setJumpInterval();
	void setWalkSpeed();
	void setTurnInterval();
};


class PropertyBoxShroom : public PropertyBoxAi
{
	
public:
	void setShroomComponent(ShroomComponent* _ai);

	void build();
	void update();


private:
	ShroomComponent* ai;

	PropertyBoxEnemy enemyBox;

	sfg::Entry::Ptr bounceHeight;

	void setBounceHeight();

};






//Particle



class PropertyBoxParticle : public PropertyBoxAi
{
public:
	void setParticleComponent(ParticleComponent* _ai);

	void build();
	void update();

private:
	ParticleComponent* ai;

	sfg::Entry::Ptr depth;
	sfg::Entry::Ptr maxParticles;
	sfg::Entry::Ptr spawnIntervall;
	sfg::Entry::Ptr spawnRadiusX;
	sfg::Entry::Ptr spawnRadiusY;
	sfg::Entry::Ptr spawnRadiusZ;
	sfg::Entry::Ptr spawnCount;
	
	sfg::Entry::Ptr sprite;
	sfg::Entry::Ptr scale;
	sfg::Entry::Ptr lifeTimeMin;
	sfg::Entry::Ptr lifeTimeMax;
	sfg::Entry::Ptr minVelocityX;
	sfg::Entry::Ptr minVelocityY;
	sfg::Entry::Ptr minVelocityZ;
	sfg::Entry::Ptr maxVelocityX;
	sfg::Entry::Ptr maxVelocityY;
	sfg::Entry::Ptr maxVelocityZ;

	sfg::Entry::Ptr fieldScaleX;
	sfg::Entry::Ptr fieldScaleY;
	sfg::Entry::Ptr fieldScaleZ;
	sfg::Entry::Ptr timeScale;


	void setDepth();
	void setMaxParticles();
	void setSpawnIntervall();
	void setSpawnRadius();
	void setSpawnCount();

	void setSprite();
	void setScale();
	void setLifeTime();
	void setMinVelocity();
	void setMaxVelocity();

	void setFieldScale();
	void setTimeScale();
};


//rock


class PropertyBoxRock: public PropertyBoxAi
{
	
public:
	void setRockComponent(RockComponent* _ai);

	void build();
	void update();


private:
	RockComponent* ai;

	PropertyBoxEnemy enemyBox;

	sfg::Entry::Ptr attackRange;
	sfg::Entry::Ptr rageNumber;

	void setAttackRange();
	void setRageNumber();

};


//bomb


class PropertyBoxBomb: public PropertyBoxAi
{
	
public:
	void setBombComponent(BombComponent* _ai);

	void build();
	void update();


private:
	BombComponent* ai;

	PropertyBoxEnemy enemyBox;

	sfg::Entry::Ptr attackRange;
	sfg::Entry::Ptr explosionRadius;

	void setAttackRange();
	void setExplosionRadius();

};



#endif