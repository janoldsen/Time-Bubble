#ifndef BACKGROUND_OBJECT_H
#define BACKGROUND_OBJECT_H

#include "SpriteComponent.h"


class BackGroundObject
{
public:

	BackGroundObject();




	void draw(Camera* camera, sf::RenderTarget* target, sf::Color color = sf::Color::White);

	void write(std::fstream& stream);
	void read(std::fstream& stream, int versionNumber);


	void setSprite(sf::Texture& texture);
	sf::Sprite getSprite();

	void setDepth(float depth);
	float getDepth();

	void setPosition(sf::Vector2f _position);
	sf::Vector2f getPosition();

	std::string getName();
	void setName(std::string _name);

	sf::Vector2f getScale();
	void setScale(sf::Vector2f _size);

	void setRotation(float _rotation);
	float getRotation();

	void setTileable(sf::Vector2f v);
	sf::Vector2f getTileable();

private:
	std::string name;
	
	SpriteComponent sprite;
	sf::Vector2f position;
	float rotation;



};


#endif