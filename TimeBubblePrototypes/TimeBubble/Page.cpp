
#include "Page.h"
#include "Button.h"
#include <iostream>

Page::~Page()
{
	for (int i = 0; i < buttons.size(); i++)
		delete buttons[i];

}


void Page::draw(sf::RenderTarget* target)
{
	for (int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->draw(target);
	}

}


void Page::update(sf::RenderWindow& window, bool controller)
{

	for (int i = 0; i < buttons.size(); i++)
	{
		sf::View view = window.getView();

		sf::Vector2f p = window.mapPixelToCoords(sf::Mouse::getPosition(window));


		//buttons[i]->update();
		if (buttons[i])
		{

			if (!controller)
			{
				if (buttons[i]->Contains(window))
				{
					buttons[i]->onMouseOver();
				}
				else
				{
					buttons[i]->onMouseLeft();

				}
			}


			buttons[i]->update();
		}

	}

}


void Page::mouseClick()
{
	for (int i =0 ; i < buttons.size(); i++)
	{
		if (buttons[i]->isActive())
			buttons[i]->onClick();
	}
}

void Page::controllerLeft()
{
	for (int i = 0;  i < buttons.size(); i++)
	{
		if (buttons[i]->isActive())
		{
			if (buttons[i]->previous)
			{
				if (buttons[i]->previous->locked == false)
				{
					buttons[i]->previous->onMouseOver();
					buttons[i]->onMouseLeft();
				}
			}
			return;
		}
	}
	buttons[0]->onMouseOver();

}

void Page::controllerRight()
{
	for (int i = 0;  i < buttons.size(); i++)
	{
		if (buttons[i]->isActive())
		{
			if (buttons[i]->next)
			{
				if (buttons[i]->next->locked == false)
				{
					buttons[i]->next->onMouseOver();
					buttons[i]->onMouseLeft();
				}
			}
			return;
		}
	}

	buttons[0]->onMouseOver();

}



void Page::addButton(Button* button)
{
	buttons.push_back(button);

}

void Page::resetButtons()
{
	for(int i = 0; i < buttons.size(); i++)
	{
		buttons[i]->onMouseLeft();
	}
}