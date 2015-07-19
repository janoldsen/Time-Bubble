#include "Button.h"
#include "GameClock.h"
#include "myMath.h"


Button::~Button()
{
}



void Button::onMouseOver()
{
	if (!active && !locked)
	{
		sprite.startAnimation(1);
		active = true;	
	}
	//if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	//	mouseDown = true;
	//else if (mouseDown)
	//	onClick();
}
void Button::onMouseLeft()
{
	if (active)
	{
		sprite.startAnimation(2);
		active = false;
		//mouseDown = false;
	}
}


void Button::onClick()
{
	sprite.startAnimation(3);
}



bool Button::Contains(sf::RenderWindow& window)
{
	
	sf::Vector2f p = window.mapPixelToCoords(sf::Mouse::getPosition(window), window.getDefaultView());
	p -= sf::Vector2f(window.getSize().x * position.x, window.getSize().y * position.y);
	
	return rect.contains(p);


}


void Button::set(std::string filePath, sf::FloatRect _rect)
{
	SCML::Data data(filePath);

	fs.load(&data);
	//imageManager->getScmlFs()->load(&data);

	//sprite.setFileSystem(imageManager->getScmlFs());
	sprite.setFileSystem(&fs);
	sprite.entity = 0;
	sprite.load(&data);
	sprite.startAnimation(0);

	rect = _rect;
}


void Button::draw(sf::RenderTarget* target)
{
	/*animation.draw(MATH::vPxtoM(position), 0.0f, 0, target);*/
	sprite.setScreen(target);
	sprite.draw(target->getSize().x * position.x, target->getSize().y * position.y,
		0.0f, 1.0f, 1.0f);

}


void Button::update()
{
	sprite.update(gameClock->getElapsedTime() * 1000.0f);
}



void Button::setPosition(sf::Vector2f _position)
{
	position = _position;
}

sf::Vector2f Button::getPosition()
{
	return position;
}


bool Button::isActive()
{
	return active;
}

void Button::setActive(bool flag)
{
	active = flag;
}