#ifndef PLATTFORM_COMPONENT_H
#define PLATTFORM_COMPONENT_H

#include "AiComponent.h"

class PlattformComponent : public AiComponent
{
public:

	PlattformComponent();

	void write(std::fstream& stream);
	void read(std::fstream& stream, int versionNumber);

	void debugDraw(Camera* camera, sf::RenderTarget* target);

	ComponentTypes getType();

	AiComponent* clone();

	void setStartPosition(sf::Vector2f _startPosition);
	//void setEndPosition(sf::Vector2f _endPosition);

	void addWayPoint(sf::Vector2f wayPoint);
	void deleteWayPoint();

	sf::Vector2f getWayPoint(int id);
	void setWayPoint(int id, sf::Vector2f wayPoint);

	float getNumWayPoints();

	//sf::Vector2f getEndPosition();

	void setVelocity(float _velocity);
	float getVelocity();

	void setStartValue(float value);
	float getStartValue();

	

	void onStateChange();

	void initialize(GameObject* _parent);

	void sendMessage(Message& msg);

	void update();
	//void preStep();
	//void postStep();

private:
	//float counter;
	//bool changed;

	float startValue;
	//sf::Vector2f endPosition;
	sf::Vector2f startPosition;

	sf::Vector2f pos1;
	sf::Vector2f pos2;

	std::vector<sf::Vector2f> wayPoints;

	int curr;

	short xScale;
	short yScale;
	
	float velocity;
	short dir;
	b2Vec2 dirN;

	void updateVelocity();
	
	void setPoints(int start, int end);


};

#endif