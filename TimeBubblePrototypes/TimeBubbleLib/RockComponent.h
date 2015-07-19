#ifndef ROCK_COMPONENT_H
#define ROCK_COMPONENT_H

#include "EnemyComponent.h"



class RockComponent : public EnemyComponent
{
public:

	RockComponent();

	void write(std::fstream& stream);
	void read(std::fstream& stream, int versionNumber);

	ComponentTypes getType();

	AiComponent* clone();

	void initialize(GameObject* _parent);

	void update();

	void sendMessage(Message& msg);

	void setAttackRange(float _attackRange);
	float getAttackRange();

	void setRageNumber(int _rageNumber);
	int getRageNumber();

private:
	float attackRange;
	bool attacking;
	float attackTimer;

	int rageNumber;

	int deathCounter;

	float attackCounter;

	bool playerDead;
	
	void attack();
};


#endif