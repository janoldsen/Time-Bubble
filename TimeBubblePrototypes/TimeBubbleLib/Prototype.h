#ifndef PROTOTYPE_H
#define PROTOTYPE_H

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include "ImageManager.h"
#include "GameObject.h"
#include <vector>
#include "ContactListener.h"
#include "Camera.h"
#include "BackGround.h"
#include "ObjectContainer.h"
#include "BgObjectContainer.h"
#include "Level.h"

#include "Defines.h"

class Prototype
{
public:

	friend PlayerComponent;

	Prototype();

	virtual void initialize();
	void initializeGameObjects();

	virtual void run();

	virtual void shutdown();
	void shutdownLevel();

	bool loadLevel(std::string filename);
	
	bool addObject(GameObject* object, bool front = false);
	bool destroyObject(GameObject* object, bool startFromFront = false);
	GameObject* findObject(std::string name);

	void addBackGroundObject(BackGroundObject* object);
	void deleteBackGroundObject(BackGroundObject* object);

	void setGameOver(int flag);
	bool isGameOver();

	void setPlayer(GameObject* _player, PlayerComponent* playerAI);
	GameObject* getPlayer();
	
	void setLevel(Level& _level);
	const Level& getLevel() const;

	void setCamera(const Camera& _camera);
	const Camera* getCamera() const;

	void setDebug(bool flag);

	void setVolume(float _volume);
	float getVolume() const;

	b2World* world;
	
	sf::RenderWindow window;
	
	virtual void stopLevel(int i);
	
	bool frame();

	virtual void setPause(bool flag);

	bool controllerActive();

protected:


	void pollEvent();


	Camera camera;
	GameObject* player;

	ObjectContainer gameObjects;
	BgObjectContainer backGroundObjects;

	Level level;
	std::string levelName;

	int gameOver;
	bool debug;
	bool pause;

	virtual void addDraw() {};

	ContactListener contactListener;
	
	float physicClock;

	float fpsClock;
	unsigned int fps;

	bool controller;

	float volume;

	static const float32	timeStep;
	static const int32		velocityIterations	= 16;
	static const int32		positionIterations	= 16;

};

#endif