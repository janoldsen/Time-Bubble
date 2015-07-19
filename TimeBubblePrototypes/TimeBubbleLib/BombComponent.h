#ifndef BOMB_COMPONENT_H
#define BOMB_COMPONENT_H


#include "EnemyComponent.h"

class BombComponent :	public EnemyComponent
{
public:
	BombComponent(void);

	void write(std::fstream& stream);
	void read(std::fstream& stream, int versionNumber);

	ComponentTypes getType();

	AiComponent* clone();

	void initialize(GameObject* _parent);
	void update();

	void setAttackRange(float _attackRange);
	float getAttackRange();

	void setExplosionRadius(float _explosionRadius);
	float getExplosionRadius();

private:

	float explosionRadius;
	b2Fixture* explosion;
	float explosionTimer;
	enum { NOT, EXPLODING, EXPLODED} exploding;

	float attackRange;
	
};


#endif