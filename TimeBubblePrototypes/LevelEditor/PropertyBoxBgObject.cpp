#include "PropertyBoxBgObject.h"
#include "LevelEditor.h"
#include <BackGroundObject.h>
#include <ImageManager.h>

void PropertyBoxBgObject::build(BackGroundObject* _object, LevelEditor* _editor)
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	object = _object;
	editor = _editor;


	sfg::Box::Ptr nameBox = sfg::Box::Create();
	{
		nameBox->Pack(sfg::Label::Create("Name:"));

		name = sfg::Entry::Create(object->getName());
		name->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxBgObject::setObjectName, this);
		nameBox->Pack(name);

	}
	box->Pack(nameBox);

	sfg::Box::Ptr positionBox = sfg::Box::Create();
	{
		positionBox->Pack(sfg::Label::Create("Position: "));


		std::stringstream ss;
		ss << object->getPosition().x;
		positionX = sfg::Entry::Create(ss.str());
		positionX->SetRequisition(sf::Vector2f(20.0f, 0.0f));
		positionX->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxBgObject::setObjectPosition, this);
		positionBox->Pack(positionX);


		ss.str("");
		ss << object->getPosition().y;
		positionY = sfg::Entry::Create(ss.str());
		positionY->SetRequisition(sf::Vector2f(20.0f, 0.0f));
		positionY->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxBgObject::setObjectPosition, this);
		positionBox->Pack(positionY);
	}
	box->Pack(positionBox, false);

	sfg::Box::Ptr spriteBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{

		std::string str = imageManager->getFileName(object->getSprite().getTexture());
		str.erase(0,4);

		spriteBox->Pack(sfg::Label::Create("Sprite:"), false, false);
		sprite = sfg::Entry::Create(str);
		sprite->GetSignal(sfg::Box::OnKeyPress).Connect(&PropertyBoxBgObject::setSprite, this);
		spriteBox->Pack(sprite);

	}
	box->Pack(spriteBox);

	sfg::Box::Ptr depthBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		std::stringstream ss;
		ss << object->getDepth();

		depthBox->Pack(sfg::Label::Create("Depth:"));
		depth = sfg::Entry::Create(ss.str());
		depth->GetSignal(sfg::Box::OnKeyPress).Connect(&PropertyBoxBgObject::setDepth, this);
		depthBox->Pack(depth);

	}
	box->Pack(depthBox);



	sfg::Box::Ptr scaleBox = sfg::Box::Create();
	{
		scaleBox->Pack(sfg::Label::Create("Scale: "));
		
		std::stringstream ss;
		ss << object->getScale().x;
		scaleX = sfg::Entry::Create(ss.str());
		scaleX->SetRequisition(sf::Vector2f(20.0f, 0.0f));
		scaleX->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxBgObject::setScale, this);
		scaleBox->Pack(scaleX);


		ss.str("");
		ss << object->getScale().y;
		scaleY = sfg::Entry::Create(ss.str());
		scaleY->SetRequisition(sf::Vector2f(20.0f, 0.0f));
		scaleY->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxBgObject::setScale, this);
		scaleBox->Pack(scaleY);
	}
	box->Pack(scaleBox, false);


	sfg::Box::Ptr tileableBox = sfg::Box::Create();
	{
		tileableBox->Pack(sfg::Label::Create("Tileable: "));
		
		std::stringstream ss;
		ss << object->getTileable().x;
		tileableX = sfg::Entry::Create(ss.str());
		tileableX->SetRequisition(sf::Vector2f(20.0f, 0.0f));
		tileableX->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxBgObject::setTileable, this);
		tileableBox->Pack(tileableX);


		ss.str("");
		ss << object->getTileable().y;
		tileableY = sfg::Entry::Create(ss.str());
		tileableY->SetRequisition(sf::Vector2f(20.0f, 0.0f));
		tileableY->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxBgObject::setTileable, this);
		tileableBox->Pack(tileableY);
	}
	box->Pack(tileableBox, false);


	sfg::Box::Ptr rotationBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		std::stringstream ss;
		ss << object->getRotation();

		rotationBox->Pack(sfg::Label::Create("Rotation:"));
		rotation = sfg::Entry::Create(ss.str());
		rotation->GetSignal(sfg::Box::OnKeyPress).Connect(&PropertyBoxBgObject::setRotation, this);
		rotationBox->Pack(rotation);

	}
	box->Pack(rotationBox);



}

void PropertyBoxBgObject::update()
{

	//if (!depth->HasFocus())
	//{
	//	std::stringstream ss;
	//	ss << object->getDepth();
	//	depth->SetText(ss.str());

	//}



	//if (!name->HasFocus())
	//	name->SetText(object->getName());

	std::stringstream ss;

	ss << object->getPosition().x;
	if (!positionX->HasFocus())
		positionX->SetText(ss.str());

	ss.str("");
	ss << object->getPosition().y;
	if (!positionY->HasFocus())
		positionY->SetText(ss.str());

}

sfg::Box::Ptr PropertyBoxBgObject::get()
{
	return box;
}


void PropertyBoxBgObject::setDepth()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss(depth->GetText());

		float _depth;

		ss >> _depth;
		object->setDepth(_depth);

		editor->resortBackGroundObject(object);

		box->GrabFocus();
	}


}



void PropertyBoxBgObject::setObjectPosition()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{

		sf::Vector2f position;

		std::stringstream ssX;
		ssX.str(positionX->GetText());
		ssX >> position.x;
		
		std::stringstream ssY;
		ssY.str(positionY->GetText());
		ssY >> position.y;
		object->setPosition(position);

		box->GrabFocus();

	}
}

void PropertyBoxBgObject::setObjectName()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		object->setName(name->GetText());

		box->GrabFocus();

	}
}



void PropertyBoxBgObject::setSprite()
{

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		object->setSprite(imageManager->getTexture("res/" + sprite->GetText()));

		box->GrabFocus();

	}


}






void PropertyBoxBgObject::setRotation()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss(rotation->GetText());

		float _rotation;

		ss >> _rotation;
		object->setRotation(_rotation);

		box->GrabFocus();
	}

}


void PropertyBoxBgObject::setScale()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{

		sf::Vector2f scale;

		std::stringstream ssX;
		ssX.str(scaleX->GetText());
		ssX >> scale.x;
		
		std::stringstream ssY;
		ssY.str(scaleY->GetText());
		ssY >> scale.y;
		object->setScale(scale);

		box->GrabFocus();

	}
}

void PropertyBoxBgObject::setTileable()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{

		sf::Vector2f tileable;

		std::stringstream ssX;
		ssX.str(tileableX->GetText());
		ssX >> tileable.x;
		
		std::stringstream ssY;
		ssY.str(tileableY->GetText());
		ssY >> tileable.y;
		object->setTileable(tileable);

		box->GrabFocus();

	}
}