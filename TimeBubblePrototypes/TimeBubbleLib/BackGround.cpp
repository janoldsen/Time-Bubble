#include "BackGround.h"


BackGround::BackGround()
{
	depth = 0;
}


void BackGround::set(sf::Texture& texture, float _depth)
{
	sprite.setTexture(texture);
	sprite.setOrigin(sprite.getTextureRect().width/2.0f, sprite.getTextureRect().height / 2.0f);
	//sprite.setScale(1.25f, 1.25f);
	depth = _depth;

}

