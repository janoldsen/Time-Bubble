#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <SFML\Graphics.hpp>

class BackGround
{
public:
	BackGround();
	void set(sf::Texture& texture, float _depth);
	sf::Sprite sprite;
	float depth;


};


#endif