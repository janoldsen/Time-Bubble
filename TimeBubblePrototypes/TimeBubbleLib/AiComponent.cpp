#include "AiComponent.h"
#include "GameObject.h"
#include "RendererComponent.h"


void AiComponent::write(std::fstream& stream)
{

}

void AiComponent::read(std::fstream& stream, int versionNumber)
{

}



ComponentTypes AiComponent::getType()
{
	return ComponentTypes::STANDARD;
}



void AiComponent::initialize(GameObject* _parent)
{
	parent = _parent;
}

void AiComponent::shutdown()
{

}

void AiComponent::update()
{

}


Prototype* AiComponent::getGame()
{
	return parent->game;
}

RendererComponent* AiComponent::getRenderer()
{
	return parent->renderer;
}


b2Body* AiComponent::getBody()
{
	return parent->body;
}


void AiComponent::setParent(GameObject* _parent)
{
	parent = _parent;
}

GameObject* AiComponent::getParent()
{
	return parent;
}