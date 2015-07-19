#ifndef SELECTOR_H
#define SELECTOR_H

#include <SFML\Graphics.hpp>
#include <Box2D\Common\b2Math.h>

#include "Selection.h"
#include <vector>

#include "EditorLevel.h"
#include <ObjectIterator.h>
#include "PropertyBoxLevel.h"
#include "SelectionLevel.h"
#include "SelectionCamera.h"

class Selector
{
public:
	Selector();

	void draw(sf::RenderTarget& target,Camera* camera);
	
	void update();

	void deleteSelection();

	
	sf::FloatRect selectionRect;
	sf::FloatRect handle;
	
	void moveSelection(sf::Vector2f& amount);

	void checkSelection(EditorLevel& level, GOIterator* objects); 

	void clearSelection();
	
	void addSelection(Selection* _selection, bool add = false);
	void selectLevel(SelectionLevel* selection, bool add = false);
	void selectCamera(SelectionCamera* selection, bool add = false);

	void unselect(Selection* _selection);


	std::vector<Selection*> selection;
	SelectionLevel* levelSelection;
	SelectionCamera* cameraSelection;
	


	bool selecting;
	bool translating;

	sfg::Desktop* desktop;
	sfg::Notebook::Ptr properties;
	sfg::Box::Ptr toolbar;



private:


};

#endif