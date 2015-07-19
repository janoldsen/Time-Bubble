#ifndef LEVEL_H
#define LEVEL_H

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <fstream>
#include "SpriteComponent.h"

class Prototype;
class Camera;

class Level
{
public:

	friend Camera;


	virtual void drawBackGround(Camera* camera, sf::RenderTarget* target, bool debug = false);
	virtual void drawGameGround(Camera* camera, sf::RenderTarget* target, bool debug = false, sf::Color color = sf::Color::Black);
	virtual void shutdown(Prototype* game);

	void setBackGround(sf::Texture& texture);
	void setBackGround(SpriteComponent& _backGround);
	
	const sf::Texture* getBackGround();

	void setGameGround(sf::Texture& texture);
	void setGameGround(SpriteComponent& _gameGround);

	const sf::Texture* getGameGround();


	void setBody(b2BodyDef* bodydef, b2World* world);
	b2Body* getBody();

	void setSize(sf::IntRect _size);
	const sf::IntRect& getSize() const;

	


	virtual void read(std::fstream& stream, Prototype* game, int versionNumber);
	void write(std::fstream& stream);


	void addFixture(b2FixtureDef* fixtureDef);

protected:
	SpriteComponent backGround;
	SpriteComponent gameGround;

	sf::Vector2f bgPosition;

	b2Body* body;

	sf::IntRect size;

	virtual void readBody(std::fstream& stream, Prototype* game);
	virtual void writeBody(std::fstream& stream);





};



#endif