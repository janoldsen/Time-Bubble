#ifndef ENEMY_COMPONENT_H
#define ENEMY_COMPONENT_H

#include "AiComponent.h"
#include "SCMLComponent.h"

class EnemyComponent : public AiComponent
{
public:
	EnemyComponent();

	void write(std::fstream& stream);
	void read(std::fstream& stream, int versionNumber);

	ComponentTypes getType();

	AiComponent* clone();

	void initialize(GameObject* _parent);
	void sendMessage(Message& msg);
	void update();

	void setJumpInterval(float _jumpInterval);
	float getJumpInterval() const;

	void setMaxJumpHeight(float _maxJumpHeight);
	float getMaxJumpHeight() const;
	
	void setTurnInterval(float _turnInterval);
	float getTurnInterval() const;

	void setWalkSpeed(float _walkSpeed);
	float getWalkSpeed() const; 


protected:
	SCMLComponent* scml;


	float jumpInterval;
	float jumpTimer;

	int numFootContacts;
	bool jumping;
	bool walking;

	float maxJumpHeight;
	b2Vec2 jumpImpulse;

	float turnInterval;
	float turnTimer;
	float walkSpeed;



};

#endif