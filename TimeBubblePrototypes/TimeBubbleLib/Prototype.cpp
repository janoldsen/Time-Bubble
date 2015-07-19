#include "Prototype.h"
#include <iostream>

#include "RendererComponent.h"
#include "BodyComponent.h"
#include "AiComponent.h"
#include "ImageManager.h"
#include "PlayerComponent.h"
#include "JumperComponent.h"
#include "GameClock.h"
#include "PlattformComponent.h"
#include "ObjectIterator.h"
#include "AnimationComponent.h"
#include "ParticleManager.h"


const float32 Prototype::timeStep = 1.0f / 60.0f;


Prototype::Prototype()
{
	//gameObjects = NULL;	

	volume = 100.0f;
}



void Prototype::initialize()
{

	if (!window.isOpen())
	{
		window.create(sf::VideoMode(800,600),"Prototype_1", sf::Style::Close);
		//window.create(sf::VideoMode::getFullscreenModes()[0],"klgfja", sf::Style::Fullscreen);
	}



	b2Vec2 gravity(0.0f, -GRAVITY);

	world = new b2World(gravity);
	world->SetContactListener(&contactListener);

	//camera.setZoom(4.0f);
	camera.initialize(&window);

	gameClock->initialize();
	physicClock	= 0;
	fpsClock = 0;
	fps = 0;


	gameOver = false;
	pause = false;
	debug = false;

	controller = false;


}

void Prototype::initializeGameObjects()
{
	GOIterator* it;
	for (it = gameObjects.begin; it; it = it->next)
	{
		it->object->initialize(this);
	}

}


void Prototype::run()
{
	while(window.isOpen())
	{

		frame();

	}

}


bool Prototype::frame()
{

	if (!window.isOpen())
		return false;

	gameClock->startNewFrame();




		
	
	pollEvent();


	if (!pause)
	{

		physicClock += gameClock->getElapsedTime();
		fpsClock += gameClock->getElapsedTime();

		//physics

		while (physicClock >= timeStep)
		{


			world->Step(timeStep, velocityIterations, positionIterations);
			world->ClearForces();

			physicClock -= timeStep;
		}


		//update

		for (GOIterator* it = gameObjects.begin; it; it = it->next)
		{
			it->object->update();
		}


	}

	// render


	camera.update(level);

	camera.draw(gameObjects.begin, &backGroundObjects, level, debug);

	addDraw();

	window.display();


	
	fps++;
	if (fpsClock >= 1.0f)
	{
		std::cout << "FPS:	" << fps << "	";
		std::cout << "Particle count:	" << particleManager->getSize() << std::endl;
		fps = 0;
		fpsClock = 0;
	}

	
	gameObjects.clean();
	
	
		
	if (gameOver)
	{
		if (gameOver == -1)
		{
			shutdownLevel();
			Prototype::initialize();
			loadLevel(levelName);
			gameOver = false;
		}
		else
		{
			stopLevel(gameOver);
		}

		return false;
	}
	
	
	return true;

}


void Prototype::shutdown()
{

	gameOver = true;

	shutdownLevel();

	window.close();

	delete world;
	world = 0;

	//player = 0;

}

void Prototype::shutdownLevel()
{
	
	
	gameObjects.shutdown();
	backGroundObjects.shutdown();
	level.shutdown(this);



	//delete world;
	//world = 0;
	//
	//player = 0;

	window.setView(window.getDefaultView());


}

bool Prototype::loadLevel(std::string filename)
{
	std::fstream levelStream(filename, std::ios::in | std::ios::binary);

	if (!levelStream.is_open())
		return false;


	int levelVersionNumber;

	levelStream.read((char*)&levelVersionNumber, sizeof(levelVersionNumber));

	level.read(levelStream, this, levelVersionNumber);

	if (levelVersionNumber >= 2)
	{
		camera.read(levelStream, levelVersionNumber);
		camera.initialize(&window);
	}

	unsigned int numberOfGameObjects;
	levelStream.read((char*)&numberOfGameObjects, sizeof(numberOfGameObjects));

	for (int i = 0; i < numberOfGameObjects; i++)
	{
		GameObject* gameObject = new GameObject();

		gameObject->read(levelStream, this, levelVersionNumber);
		addObject(gameObject);
		//gameObject->initialize(this);

	}

	initializeGameObjects();

	if (levelVersionNumber >= 2)
	{
		unsigned int nBg;
		levelStream.read((char*)&nBg, sizeof(nBg));

		for (int i = 0; i < nBg; i++)
		{
			BackGroundObject* bgObject = new BackGroundObject();
			bgObject->read(levelStream, levelVersionNumber);
			addBackGroundObject(bgObject);
		}
	}



	levelStream.close();

	levelName = filename;

	gameClock->startNewFrame();

}

void Prototype::stopLevel(int i)
{
	shutdown();
}


void Prototype::pollEvent()
{
	sf::Event event;

	while(window.pollEvent(event))
	{

		if (event.type == sf::Event::LostFocus)
			setPause(true);

		if (event.type == sf::Event::GainedFocus)
			setPause(false);

		if (event.type == sf::Event::Closed)
			window.close();

		if (event.type == sf::Event::MouseMoved)
		{
			controller = false;
		}

		if (event.type == sf::Event::KeyPressed)
		{
			if (event.key.code == sf::Keyboard::R)
			{
				gameOver = -1;

			}
			else if (event.key.code == sf::Keyboard::F)
				debug = !debug;
			else if (event.key.code == sf::Keyboard::Escape)
				setPause(!pause);

			controller = false;

		}

		if (event.type == sf::Event::JoystickButtonPressed)
		{
			if (event.joystickButton.button == 6)
				gameOver = -1;

			controller = true;
		}

		if (event.type == sf::Event::JoystickMoved)
		{
			controller = true;
		}
	}
}

bool Prototype::addObject(GameObject* object, bool front)
{

	//object->initialize(this);

	gameObjects.addObject(object, front);

	return true;
}

bool Prototype::destroyObject(GameObject* object, bool startFromFront)
{
	gameObjects.deleteObject(object, startFromFront);

	return false;

}

GameObject* Prototype::findObject(std::string name)
{
	return gameObjects.find(name);
}

void Prototype::addBackGroundObject(BackGroundObject* object)
{
	backGroundObjects.addObject(object);
}
void Prototype::deleteBackGroundObject(BackGroundObject* object)
{
	backGroundObjects.deleteObject(object);
}


void Prototype::setGameOver(int flag)
{
	gameOver = flag;
}

bool Prototype::isGameOver()
{
	return gameOver;
}


void Prototype::setPlayer(GameObject* _player, PlayerComponent* playerAi)
{
	player = _player;

	camera.setPlayer(_player, playerAi);
}

GameObject* Prototype::getPlayer()
{
	return player;
}



void Prototype::setLevel(Level& _level)
{
	level = _level;
}

const Level& Prototype::getLevel() const
{
	return level;
}


void Prototype::setCamera(const Camera& _camera)
{
	camera.setFogColor(_camera.getFogColor());
	camera.setFogStrength(_camera.getFogStrength());
	camera.setClouds(_camera.getClouds());
	camera.setZoom(_camera.getZoom());
	camera.setSunStrength(_camera.getSunStrength());

	camera.initialize(&window);
}

const Camera* Prototype::getCamera() const
{
	return &camera;
}



void Prototype::setPause(bool flag)
{
	pause = flag;
}


bool Prototype::controllerActive()
{
	return controller;
}


void Prototype::setDebug(bool flag)
{
	debug = flag;
}

void Prototype::setVolume(float _volume)
{
	volume = _volume;
}


float Prototype::getVolume() const
{
	return volume;
}