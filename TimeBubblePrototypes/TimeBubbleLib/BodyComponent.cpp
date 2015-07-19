#include "BodyComponent.h"

#include "GameObject.h"


void BodyComponent::initialize(GameObject* _parent)
{
	parent = _parent;
	//world = _world;


}


void BodyComponent::shutdown()
{
	world->DestroyBody(body);
}

void BodyComponent::update()
{


}


sf::Vector2<float> BodyComponent::getPosition()
{
	sf::Vector2<float> _return;

	_return.x = body->GetPosition().x;
	_return.y = body->GetPosition().y;

	return _return;
}

void BodyComponent::setBody(b2Body* _body)
{
	body = _body;
}

void BodyComponent::setWorld(b2World* _world)
{
	world = _world;
}


b2Body* BodyComponent::getBody()
{
	return body;
}