#include "SelectionBgObject.h"
#include <BackGroundObject.h>
#include "LevelEditor.h"

SelectionBgObject::SelectionBgObject()
{
}


SelectionBgObject::SelectionBgObject(LevelEditor* _editor, BackGroundObject* _object, Selector* selector, sfg::Box::Ptr* hierarchy )
{
	editor = _editor;
	object = _object;

	Selection::set(selector, hierarchy);

	propertyBox.build(object, editor);

	_hasProperty = true;

}


void SelectionBgObject::move(sf::Vector2f amount)
{
	object->setPosition(object->getPosition() + amount);
}

std::string SelectionBgObject::getName()
{
	return object->getName();
}


sf::Vector2f SelectionBgObject::getPosition()
{
	return object->getPosition();
}



sfg::Box::Ptr SelectionBgObject::getPropertyBox()
{
	return propertyBox.get();
}


void SelectionBgObject::updateProperty()
{
	propertyBox.update();

	updateHierarchyButton();
}



const BackGroundObject* SelectionBgObject::getObject() const
{
	return object;
}



void SelectionBgObject::deleteSelection()
{
	selector->unselect(this);
	editor->deleteBackGroundObject(object);
}