#ifndef SELECTION_OBJECT_H
#define SELECTION_OBJECT_H

#include "Selection.h"
//#include "LevelObject.h"
#include "PropertyBoxObject.h"

class LevelObject;
class LevelEditor;

class SelectionObject : public Selection
{
public:

	SelectionObject();

	SelectionObject(LevelObject* _object, sf::Vector2f* _position);

	void shutdown();

	void deleteSelection();

	void move(sf::Vector2f amount);
	
	void set(LevelEditor* _editor, LevelObject* _object, sf::Vector2f* _position, Selector* _selector, sfg::Box::Ptr* hierarchy);
	

	std::string getName();

	sf::Vector2f getPosition();
	
	sfg::Box::Ptr getPropertyBox();

	
	void updateProperty();

private:
	sf::Vector2f* position;

	LevelEditor* editor;
	LevelObject* object;

	PropertyBoxObject propertyBox;

	//sfg::Entry::Ptr positionX;
	//sfg::Entry::Ptr positionY;



};

#endif