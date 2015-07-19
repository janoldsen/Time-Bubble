#ifndef SHROOMCOMPONENT_H
#define SHROOMCOMPONENT_H

#include "EnemyComponent.h"


class ShroomComponent :	public EnemyComponent
{
public:
	ShroomComponent();

	ComponentTypes getType();

	void update();

	void write(std::fstream& stream);
	void read(std::fstream& stream, int versionNumber);

	AiComponent* clone();

	void sendMessage(Message& msg);

	void setBounceHeight(float _bounceHeight);
	float getBounceHeight() const;

private:
	
	float bounceHeight;
	float bounceCd;
	float attackCd;


};


#endif