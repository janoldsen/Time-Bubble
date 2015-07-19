#ifndef SWING_COMPONENT_H
#define SWING_COMPONENT_H

#include "AiComponent.h"
#include "SCMLComponent.h"

class SwingComponent : public AiComponent
{
public:
	SwingComponent();

	AiComponent* clone();
	void update();

	ComponentTypes getType();

	void initialize(GameObject* _parent);

	void write(std::fstream& stream);
	void read(std::fstream& stream, int versionNumber);

	void setMaxAngle(float angle);
	float getMaxAngle();

	void setVelocity(float _velocity);
	float getVelocity();

	void setStartAngle(float angle);
	float getStartAngle();

	void sendMessage(Message& msg);


	void onStateChange();

private:
	float timer;

	float startAngle;

	SCMLComponent* scml;

	float maxAngle;
	float velocity;

	bool player;

};


#endif