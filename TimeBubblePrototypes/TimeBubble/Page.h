#ifndef PAGE_H
#define PAGE_H

#include "RendererComponent.h"
//#include "Button.h"
#include <vector>

class Button;

class Page
{
public:

	Page()
	{
		lastPage = NULL;
	}
	~Page();

	void draw(sf::RenderTarget* target);
	void update(sf::RenderWindow& window, bool controller = false);
	void mouseClick();

	void controllerLeft();
	void controllerRight();


	void addButton(Button* button);
	void setBackground();

	void resetButtons();

	Page* lastPage;


	
private:

	std::vector<Button*> buttons;



};

#endif