#include "SpriteComponent.h"

#include "GameObject.h"
#include "BodyComponent.h"
#include "Defines.h"
#include "ImageManager.h"
#include "Camera.h"
#include <iostream>
#include "myMath.h"



SpriteComponent::SpriteComponent()
{
	depth = 1.0f;
	depthScale = false;
}


void SpriteComponent::write(std::fstream& stream)
{
	std::string filename = imageManager->getFileName(sprite.getTexture());
	
	int size = filename.length();

	stream.write((char*)&size, sizeof(size));
	stream.write(filename.c_str(), size);

	sf::Vector2f origin = sprite.getOrigin();
	stream.write((char*)&origin, sizeof(origin));

	stream.write((char*)&depth, sizeof(depth));
	stream.write((char*)&depthScale, sizeof(depthScale));

	//stream.write((char*)&filename, sizeof(filename));

	sf::Vector2f scale = getScale();
	stream.write((char*)&scale, sizeof(scale));


}

void SpriteComponent::read(std::fstream& stream, int levelVersion)
{
	//std::string spriteFilename;
	
	//char buffer[64];
	int size;

	stream.read((char*)&size, sizeof(size));
	std::string filepath(size, '\0');
	stream.read(&filepath[0], size * sizeof(char));
	
	setSprite(imageManager->getTexture(filepath));

	sf::Vector2f origin;
	stream.read((char*)&origin, sizeof(origin));
	sprite.setOrigin(origin);

	float _depth;
	bool isScaling;


	stream.read((char*)&_depth, sizeof(_depth));
	stream.read((char*)&isScaling, sizeof(isScaling));

	setDepth(_depth, isScaling);

	if (levelVersion >= 10)
	{
		sf::Vector2f scale;
		stream.read((char*)&scale, sizeof(scale));
		setScale(scale);
	}
}

ComponentTypes SpriteComponent::getType()
{
	return ComponentTypes::STANDARD;
}


RendererComponent* SpriteComponent::clone()
{
	return new SpriteComponent(*this);
}


void SpriteComponent::initialize(/*GameObject* _parent*/)
{

	//parent = _parent;

	/*sprite.setOrigin(sprite.getTextureRect().width/2.0f, sprite.getTextureRect().height / 2.0f);*/

}

void SpriteComponent::shutdown()
{

}

void SpriteComponent::update(float timescale)
{

}


void SpriteComponent::draw(sf::Vector2f position, float angle, Camera* camera, sf::RenderTarget* target, sf::Color color)
{
	//sf::Vector2f position;
	//if (parent)
	//	 position = parent->getPosition();

	sprite.setRotation(angle * -180.0f/b2_pi);

	sprite.setColor(color);


	if (depth == 0)
	{
		std::cout << "ERROR depth == 0" << std::endl;
		return;
	}

	if (depth == 1 || !camera->isDepthEnabled())
	{
		//sprite.setColor(color);
		sprite.setPosition(position.x * mToPx, position.y * -mToPx);
		target->draw(sprite);
	}
	else
	{
		sf::Vector2f cP = camera->getPosition();
		sprite.setPosition((cP.x -  (cP.x - position.x * mToPx)/depth), (cP.y - (cP.y - position.y * -mToPx)/depth));

		sf::Shader* shader = camera->getRenderShader();

		shader->setParameter("texture", sf::Shader::CurrentTexture);
		shader->setParameter("depth", depth);

		target->draw(sprite, shader);
	}



	
	
}

void SpriteComponent::setSprite(sf::Texture& texture)
{
	sprite.setTexture(texture, true);
	sprite.setOrigin(sprite.getTextureRect().width/2.0f, sprite.getTextureRect().height / 2.0f);
}

sf::Sprite SpriteComponent::getSprite()
{
	return sprite;
}

void SpriteComponent::setOrientation(Orientation _orientation)
{
	sprite.setScale(abs(sprite.getScale().x) * (int)(_orientation-2),sprite.getScale().y);
}


void SpriteComponent::setDepth(float _depth, bool scale)
{
	
	if (depthScale)
		sprite.scale(depth, depth);

	depth = _depth;

	depthScale = scale;

	
	if (scale)
		sprite.scale(1.0f/depth, 1.0f/depth);

}

float SpriteComponent::getDepth()
{
	return depth;
}


void SpriteComponent::setOrigin(sf::Vector2f origin)
{
	sprite.setOrigin(origin.x * mToPx, origin.y * -mToPx);

}


sf::Vector2f SpriteComponent::getOrigin()
{
	return MATH::vPxtoM(sprite.getOrigin());
}

void SpriteComponent::setScale(sf::Vector2f scale)
{
	sf::Vector2f oldScale = sprite.getScale();

	oldScale *= depth;


	sprite.scale(1.0 / oldScale.x, 1.0 / oldScale.y);

	sprite.scale(scale);

}


sf::Vector2f SpriteComponent::getScale()
{
	return sprite.getScale() * depth;
}



void SpriteComponent::setTileable(sf::Vector2f v)
{
	sf::Vector2u texSize = sprite.getTexture()->getSize();

	sprite.setTextureRect(sf::IntRect(0.0f, 0.0f, texSize.x * v.x, texSize.y * v.y));
}


sf::Vector2f SpriteComponent::getTileable()
{
	if (sprite.getTexture())
	{
		sf::Vector2u texSize = sprite.getTexture()->getSize();

		if (texSize.x != 0 && texSize.y != 0)
			return sf::Vector2f(sprite.getTextureRect().width / texSize.x,
				sprite.getTextureRect().height / texSize.y);
		else
			return sf::Vector2f(1.0f,1.0f);
	}
	else 
		return sf::Vector2f(1.0f,1.0f);

}