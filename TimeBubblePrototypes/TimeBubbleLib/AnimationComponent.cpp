#include "AnimationComponent.h"
#include "GameClock.h"
#include <iostream>
#include "GameObject.h"
#include "ImageManager.h"



AnimationComponent::AnimationComponent()
{
	parent = 0;
	mode = STOP;
}


void AnimationComponent::setParent(GameObject* _parent)
{
	parent = _parent;
}


void AnimationComponent::write(std::fstream& stream)
{
	std::string filename = imageManager->getFileName(sprite.getTexture());
	//sf::Vector2i _spriteSize = spriteSize;

	int size = filename.size();

	stream.write((char*)&size, sizeof(size));
	stream.write(filename.c_str(), size * sizeof(char));
	stream.write((char*)&spriteSize, sizeof(spriteSize));
}

void AnimationComponent::read(std::fstream& stream, int levelVersion)
{
	//std::string spriteFilename;
	int size;
	//char buffer[64];
	sf::Vector2i _spriteSize;
	
	stream.read((char*)&size, sizeof(size));
	std::string filepath(size,'\0');
	stream.read(&filepath[0], sizeof(char) * size);

	
	//stream.read(buffer, size);
	stream.read((char*)&_spriteSize, sizeof(_spriteSize));


	setSpriteSheet(imageManager->getTexture(filepath), _spriteSize);

}


ComponentTypes AnimationComponent::getType()
{
	return ComponentTypes::ANIMATION;
}


RendererComponent* AnimationComponent::clone()
{

	return new AnimationComponent(*this);

}


void AnimationComponent::initialize()
{
	SpriteComponent::initialize();

	currPosition.x = 0;
	currPosition.y = 0;

	clock = 0.0f;

	animation = NULL;

	mode = STOP;

	frameCounter = 0;
}


void AnimationComponent::setSpriteSheet(sf::Texture& texture, sf::Vector2i _spriteSize)
{
	spriteSize = _spriteSize;

	sprite.setTexture(texture);
	sprite.setTextureRect(sf::Rect<int>(currPosition, spriteSize));

	sprite.setOrigin((float)spriteSize.x/2.0f, (float)spriteSize.y / 2.0f);

}



void AnimationComponent::play(Animation* _animation)
{
	if (animation == _animation)
		return;

	animation = _animation;
	firstFrame = true;
	frameCounter = 0;
	mode = PLAY;
}

void AnimationComponent::loop(Animation* _animation)
{
	if (animation == _animation)
		return;
	animation = _animation;
	firstFrame = true;
	frameCounter = 0;
	mode = LOOP;
}




void AnimationComponent::draw(sf::Vector2f position, float angle,  Camera* camera, sf::RenderTarget* target, sf::Color color)
{
	//if (!animation)
	//{
	//	RendererComponent::draw(window);
	//	return;
	//}


	if (mode == STOP)
	{
		SpriteComponent::draw(position, angle,  camera, target, color);
		return;
	}



	
	if (parent)
		clock += gameClock->getElapsedTime() * parent->getState();
	else
		clock += gameClock->getElapsedTime();

	while (clock >= animation->duration/(float)animation->numberOfFrames)
	{

		if (frameCounter >= animation->numberOfFrames)
		{
			if (mode == LOOP)
			{
				currPosition = animation->startPosition;
				frameCounter = 0;
			}
			else if (mode == PLAY)
			{
				mode = STOP;
				animation = NULL;
				return;
			}


		}
		else if (firstFrame)
		{
			currPosition = animation->startPosition;
			firstFrame = false;
		}
		else
		{
			currPosition.x += spriteSize.x;
		}
		sprite.setTextureRect(sf::Rect<int>(currPosition, spriteSize));
		clock -= animation->duration/(float)animation->numberOfFrames;
		frameCounter++;

	}
	SpriteComponent::draw(position, angle, camera, target, color);
}