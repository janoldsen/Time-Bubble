#ifndef SELECTION_H
#define SELECTION_H


#include <SFML\Graphics.hpp>
#include <SFGUI\SFGUI.hpp>
#include <string>
#include "PropertyBox.h"
//#include "Selector.h"

class Selector;

class Selection
{
public:

	
	Selection();
	virtual void move(sf::Vector2f amount) = 0;
	virtual sf::Vector2f getPosition() = 0;

	virtual void deleteSelection();
	
	virtual std::string getName() = 0;
	
	virtual void select();
	virtual void select(bool add);
	virtual void unSelect();

	void set(Selector* _selector, sfg::Box::Ptr* hierarchy);

	bool hasProperty();
	bool hasToolbar();

	virtual sfg::Box::Ptr getPropertyBox();
	virtual sfg::Box::Ptr getToolbarBox();

	virtual void updateProperty();
	

	void updateHierarchyButton();

	bool isBuilt();

	sfg::ToggleButton::Ptr hierarchyButton;

protected:

	bool built;

	Selector* selector;

	bool _hasProperty;
	bool _hasToolbar;

	sfg::Box::Ptr propertyBox;
	sfg::Box::Ptr toolbarBox;
	//PropertyBox propertyBox;
	



};


#endif