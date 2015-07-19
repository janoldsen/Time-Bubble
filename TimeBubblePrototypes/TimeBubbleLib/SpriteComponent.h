#ifndef SPRITE_COMPONENT_H
#define SPRITE_COMPONENT_H

#include <SFML\Graphics.hpp>
#include "GameObject.h"
#include <fstream>
#include "RendererComponent.h"


class GameObject;
class Camera;

class SpriteComponent : public RendererComponent
{
public:

	SpriteComponent();

	virtual void write(std::fstream& stream);
	virtual void read(std::fstream& stream, int levelVersion);

	virtual ComponentTypes getType();

	virtual RendererComponent* clone();

	virtual void initialize();
	void shutdown();


	void update(float timescale);
	void draw(sf::Vector2f position, float angle, Camera* camera, sf::RenderTarget* target, sf::Color color = sf::Color::White);

	void setSprite(sf::Texture& texture);
	sf::Sprite getSprite();


	void setOrientation(Orientation _orientation);

	void setDepth(float _depth, bool scale = true);
	float getDepth();

	void setScale(sf::Vector2f scale);
	sf::Vector2f getScale();

	void setOrigin(sf::Vector2f origin);
	sf::Vector2f getOrigin();

	void setTileable(sf::Vector2f v);
	sf::Vector2f getTileable();


protected:

	//short orientation;
	sf::Sprite sprite;


	float depth;
	bool depthScale;

};

#endif