#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include <string>
#include <SFML\Graphics.hpp>
#include "Message.h"
#include <Box2D\Box2D.h>
#include <fstream>

class BodyComponent;
class RendererComponent;
class AiComponent;
class Prototype;
class Camera;

enum State
{
	NONE = 0,
	SLOW,
	NORMAL,
	FAST
};


enum FixtureType
{
	ZERO = 0,
	PLAYER = 1,
	ENEMY = 2,
	FOOT = 4,
	GROUND = 8,
	DYNAMIC = 16,
	DEADLY = 32,
	BUBBLE = 64,
	CRUSHING = 128,
	BUBBLE_INNER = 256,
	BOUNCE = 512,
};



enum class ComponentTypes
{
	ZERO = 0,
	STANDARD = 1,
	ANIMATION,
	JUMPER,
	PLATTFORM,
	PLAYER,
	SWING,
	BUBBLE,
	EVENT,
	SCML,
	ENEMY,
	SHROOM,
	PARTICLE,
	ROCK,
	BOMB


};




enum Orientation
{
	LEFT = 1, 
	RIGHT = 3
};


class GameObject
{



	friend AiComponent;

public:
	//GameObject();
	GameObject(std::string _name = "Unnamed");

	virtual ~GameObject() {};


	void write(std::fstream& stream);
	void read(std::fstream& stream, Prototype* game, int versionNumber);


	void initialize(Prototype* _game);
	void update();
	virtual void shutdown();

	virtual void draw(Camera* camera, sf::RenderTarget* target, bool debug = false, sf::Color color = sf::Color::White);


	b2Body* setBody(b2BodyDef* bodyDef, b2World* world);
	const b2Body* getBody() const;

	void setRendererComponent(RendererComponent* rendererComponent);

	void setAiComponent(AiComponent* aiComponent);
	//AiComponent* getAiComponent();



	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f position);

	float getAngle();
	void setAngle(float _angle);

	Orientation getOrientation();
	void setOrientation(Orientation _orientation);

	virtual void setName(std::string _name);
	std::string getName();

	void setCenter(sf::Vector2f center);
	sf::Vector2f getCenter();

	void setState(State _state);
	void setNextState(State _state);
	State getState();
	
	float getTimescale();


	void sendMessage(Message& msg);

	b2Fixture* addFixture(b2FixtureDef* fixtureDef);
	
	bool testPoint(b2Vec2 point);

	void setActive(bool flag);

protected:


	virtual void writeBody(std::fstream& stream);
	virtual void readBody(std::fstream& stream, Prototype* _game, int versionNumber);

	virtual void writeRenderer(std::fstream& stream);
	virtual void readRenderer(std::fstream& stream, int versionNumber);

	virtual void writeAi(std::fstream& stream);
	virtual void readAi(std::fstream& stream, int versionNumber);

	sf::Vector2f position;
	sf::Vector2f center;
	Orientation orientation;
	float angle;


	bool bodyIsActive;
	b2Body* body;

	Prototype* game;

	std::string name;
	
	bool aiIsActive;
	AiComponent* ai;
	RendererComponent* renderer;

	State state;
	State nextState;


	void doClone();
	bool clone;
	sf::Vector2f clonePosition;
	float cloneAngle;
	

};

#endif