#ifndef BUTTON_H
#define BUTTON_H

#include "Game.h"
#include "SCML_SFML.h"
#include "ImageManager.h"
#include <iostream>


class Button
{
public:
	Button() {mouseDown = false; active = false; locked = false;}
	virtual ~Button();

	void onMouseOver();
	void onMouseLeft();
	virtual void onClick();


	bool Contains(sf::RenderWindow& window);
	
	void setPosition(sf::Vector2f _position);
	sf::Vector2f getPosition();

	void set(std::string filePath, sf::FloatRect _rect);
	

	virtual void draw(sf::RenderTarget* target);
	virtual void update();

	bool isActive();
	void setActive(bool flag);

	bool locked;

	Button* next;
	Button* previous;

protected:

	bool active;
	bool mouseDown;

	sf::FloatRect rect;
	sf::Vector2f position;
	
	SCML_SFML::FileSystem fs;
	SCML_SFML::Entity sprite;



};


class PageButton : public Button
{
public:
	Game* game;
	Page* page;
	void onClick() {game->setPage(page);}
};


class WorldButton : public PageButton
{
public:
	int numRequiredStar;
	int* currNumStar;

	void onClick() 
	{
		if (*currNumStar >= numRequiredStar)
			game->setPage(page);
	}
	
	void draw(sf::RenderTarget* target)
	{
		Button::draw(target);

		if (*currNumStar < numRequiredStar && !locked)
		{

			sf::Sprite starSpr(imageManager->getTexture("res/buttons/star.png"));
			starSpr.setOrigin(50.0f,50.0f);
			starSpr.setPosition(target->getSize().x * position.x, target->getSize().y * position.y);
			starSpr.setColor(sf::Color::Black);
			target->draw(starSpr);
			starSpr.setColor(sf::Color::White);
			starSpr.setTextureRect(sf::IntRect(0,0, *currNumStar/(float)numRequiredStar * 100,100));
			target->draw(starSpr);
		}
	}

};


class LevelButton : public Button
{
public:
	Game* game;
	int worldId;
	int levelId;
	bool star;
	void onClick() {game->startLevel(worldId, levelId);}
	void draw(sf::RenderTarget* target)
	{
		Button::draw(target);

		if (star)
		{
			sf::Sprite starSpr(imageManager->getTexture("res/buttons/star.png"));
			starSpr.setScale(0.25f,0.25f);
			starSpr.setPosition(target->getSize().x * position.x - 35, target->getSize().y * position.y - 35);
			target->draw(starSpr);
		}


	}
};


class ResetButton : public Button
{
public:
	Game* game;
	void onClick() {game->setGameOver(-1); }
};

class StopLevelButton : public Button
{
public:
	Game* game;
	void onClick() {game->stopLevel(0);}
};


class QuitButton : public Button
{
public:
	Game* game;
	void onClick() {game->shutdown();}

};

class setWindowButton : public Button
{
public:
	Game* game;
	sf::VideoMode videoMode;
	bool fullscreen;
	void onClick() {game->setWindow(videoMode, fullscreen);}
};


class SavegameButton : public Button
{
public:
	Game* game;
	int id;
	void onClick() {game->loadSaveGame(id);}
};

class VolumeButton : public Button
{
public:
	Game* game;
	sf::Sprite sprite;

	void update()
	{
		if (active && sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			float x = game->window.mapPixelToCoords(sf::Mouse::getPosition(game->window)).x;
			x -= position.x * game->window.getSize().x + rect.left;
			
			
			game->setVolume(x / rect.width * 100.0f);

			sprite.setTextureRect(sf::IntRect(0.0f,0.0f,x, sprite.getTexture()->getSize().y));

		}
	}

	void draw(sf::RenderTarget* target)
	{
		sprite.setOrigin(sprite.getTexture()->getSize().x/2.0f, sprite.getTexture()->getSize().y/2.0f);
		sprite.setPosition(position.x * target->getSize().x, position.y * target->getSize().y);

		target->draw(sprite);
	}
	void set(sf::FloatRect _rect)
	{
		rect = _rect;
	}
};

#endif

