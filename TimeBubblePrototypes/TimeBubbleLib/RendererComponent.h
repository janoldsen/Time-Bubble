#ifndef RENDERER_COMPONENT_H
#define RENDERER_COMPONENT_H

#include <SFML\Graphics.hpp>
#include "GameObject.h"
#include <fstream>
#include "Message.h"

class GameObject;
class Camera;

class RendererComponent
{
public:

	RendererComponent() {};
	virtual ~RendererComponent() {};

	virtual void write(std::fstream& stream) = 0;
	virtual void read(std::fstream& stream, int levelVersion) = 0;

	virtual ComponentTypes getType() = 0;

	virtual RendererComponent* clone() = 0;

	virtual void initialize() = 0;
	virtual void shutdown() = 0;
	virtual void update(float timeScale) = 0;

	virtual void draw(sf::Vector2f position, float angle, Camera* camera, sf::RenderTarget* target, sf::Color color = sf::Color::White) = 0;

	virtual void setOrientation(Orientation _orientation) = 0;

	virtual void sendMessage(Message& msg) {};

protected:


};

#endif