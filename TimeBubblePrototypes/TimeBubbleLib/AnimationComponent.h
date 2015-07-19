#ifndef ANIMATION_COMPONENT_H
#define ANIMATION_COMPONENT_H

#include "SpriteComponent.h"
#include "Animation.h"
#include <SFML\Graphics.hpp>

//class Animation;

class AnimationComponent :	public SpriteComponent
{
public:

	AnimationComponent();

	void write(std::fstream& stream);
	void read(std::fstream& stream, int levelVersion);

	ComponentTypes getType();

	RendererComponent* clone();


	void initialize(/*GameObject* _parent*/);

	void setParent(GameObject* parent);

	void setSpriteSheet(sf::Texture& texture, sf::Vector2i _spriteSize);
	void draw(sf::Vector2f position, float angle, Camera* camera, sf::RenderTarget* target, sf::Color color = sf::Color::White);
	void play(Animation* _animation);
	void loop(Animation* _animation);


private:

	enum AnimationMode
	{
		PLAY,
		LOOP,
		STOP
	};

	GameObject* parent;

	sf::Vector2i spriteSize;
	sf::Vector2i currPosition;

	Animation* animation;

	bool firstFrame;
	

	AnimationMode mode;

	float clock;
	int frameCounter;


};

#endif