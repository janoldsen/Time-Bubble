#include "SelectionObject.h"
#include "LevelObject.h"
#include <sstream>
#include "LevelEditor.h"

SelectionObject::SelectionObject()
{

}


SelectionObject::SelectionObject(LevelObject* _object, sf::Vector2f* _position)
{
	object = _object;
	position = _position;

}

void SelectionObject::shutdown()
{
	propertyBox.shutdown();
}


void SelectionObject::deleteSelection()
{
	selector->unselect(this);
	editor->deleteObject(object);
}


void SelectionObject::move(sf::Vector2f amount)
{
	*position += amount;
}

void SelectionObject::set(LevelEditor* _editor, LevelObject* _object, sf::Vector2f* _position, Selector* _selector, sfg::Box::Ptr* hierarchy)
{
	editor = _editor;
	object = _object;
	position = _position;

	_hasProperty = true;

	propertyBox.setObject(_object);
	propertyBox.build();


	Selection::set(_selector, hierarchy);

}

//void SelectionObject::setObjectPosition()
//{
//	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
//	{
//
//		sf::Vector2f position;
//
//		std::stringstream ssX;
//		ssX.str(positionX->GetText());
//		ssX >> position.x;
//		
//		std::stringstream ssY;
//		ssY.str(positionY->GetText());
//		ssY >> position.y;
//		object->setPosition(position);
//
//		propertyBox->GrabFocus();
//
//	}
//}


sf::Vector2f SelectionObject::getPosition()
{
	return *position;
}


std::string SelectionObject::getName()
{
	return object->getName();
}


void SelectionObject::updateProperty()
{
	//sf::Vector2f position;
	//std::stringstream ss;

	//ss << object->getPosition().x;
	//if (!positionX->HasFocus())
	//	positionX->SetText(ss.str());

	//ss.str("");
	//ss << object->getPosition().y;
	//if (!positionY->HasFocus())
	//	positionY->SetText(ss.str());

	propertyBox.update();

}

sfg::Box::Ptr SelectionObject::getPropertyBox()
{
	return propertyBox.get();
}

