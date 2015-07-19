#include "BackGroundObject.h"




BackGroundObject::BackGroundObject()
{
	rotation = 0.0f;
	//sprite = new SpriteComponent();

}



void BackGroundObject::draw(Camera* camera, sf::RenderTarget* target, sf::Color color)
{
	sprite.draw(position, rotation * -b2_pi/180.0f, camera, target, color);
}



void BackGroundObject::write(std::fstream& stream)
{
	int nameSize = name.size();
	stream.write((char*)&nameSize, sizeof(nameSize));
	stream.write(name.c_str(), nameSize);
	
	stream.write((char*)&position, sizeof(position));

	sprite.write(stream);


	//sf::Vector2f scale = getScale();

	//stream.write((char*)&scale, sizeof(scale));

	stream.write((char*)&rotation, sizeof(rotation));

	sf::Vector2f tileable = sprite.getTileable();
	stream.write((char*)&tileable, sizeof(tileable));

}


void BackGroundObject::read(std::fstream& stream, int versionNumber)
{
	int nameSize;
	//char buffer[16];

	stream.read((char*)&nameSize, sizeof(nameSize));
	//stream.read(buffer, nameSize);
	name = std::string(nameSize, '\0');
	stream.read(&name[0], nameSize * sizeof(char));


	stream.read((char*)&position, sizeof(position));

	sprite.read(stream, versionNumber);

	if (versionNumber < 10)
	{
		sf::Vector2f scale;

		stream.read((char*)&scale, sizeof(scale));

		sprite.setScale(scale);
	}

	stream.read((char*)&rotation, sizeof(rotation));


	if (versionNumber >= 9)
	{
		sf::Vector2f tileable;
		stream.read((char*)&tileable, sizeof(tileable));
		sprite.setTileable(tileable);
	}

}




void BackGroundObject::setSprite(sf::Texture& texture)
{
	sprite.setSprite(texture);
}

sf::Sprite BackGroundObject::getSprite()
{
	return sprite.getSprite();
}

void BackGroundObject::setDepth(float depth)
{
	sprite.setDepth(depth);
}

float BackGroundObject::getDepth()
{
	return sprite.getDepth();
}




void BackGroundObject::setPosition(sf::Vector2f _position)
{
	position = _position;
}


sf::Vector2f BackGroundObject::getPosition()
{
	return position;
}



std::string BackGroundObject::getName()
{
	return name;
}

void BackGroundObject::setName(std::string _name)
{
	name = _name;
}



sf::Vector2f BackGroundObject::getScale()
{
	return sprite.getSprite().getScale() * sprite.getDepth();
}


void BackGroundObject::setScale(sf::Vector2f scale)
{
	sprite.setScale(scale);
}


void BackGroundObject::setRotation(float _rotation)
{
	rotation = _rotation;
}

float BackGroundObject::getRotation()
{
	return rotation;
}



void BackGroundObject::setTileable(sf::Vector2f v)
{
	sprite.setTileable(v);
}


sf::Vector2f BackGroundObject::getTileable()
{
	return sprite.getTileable();
}