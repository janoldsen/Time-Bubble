#ifndef BODY_COMPONENT_H
#define BODY_COMPONENT_H

#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>


class GameObject;


class BodyComponent
{
public:
	void initialize(GameObject* _parent);
	void update();
	void shutdown();

	void setBody(b2Body* _body);
	void setWorld(b2World* _world);

	sf::Vector2<float> getPosition();
	
	b2Body* getBody();

	

private:
	
	GameObject* parent;
	
	b2World* world;
	b2Body* body;



	

};

#endif