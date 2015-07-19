#ifndef SELECTION_BG_OBJECT_H
#define SELECTION_BG_OBJECT_H

#include "Selection.h"
#include "PropertyBoxBgObject.h"

class LevelEditor;
class Selector;
class BackGroundObject;

class SelectionBgObject : public Selection
{
public:
	SelectionBgObject();

	SelectionBgObject(LevelEditor* _editor, BackGroundObject* _object, Selector* selector, sfg::Box::Ptr* hierarchy );

	void move(sf::Vector2f amount);

	void deleteSelection();

	std::string getName();
	sf::Vector2f getPosition();

	sfg::Box::Ptr getPropertyBox();

	void updateProperty();

	const BackGroundObject* getObject() const;


private:

	LevelEditor* editor;
	BackGroundObject* object;

	PropertyBoxBgObject propertyBox;


};

#endif