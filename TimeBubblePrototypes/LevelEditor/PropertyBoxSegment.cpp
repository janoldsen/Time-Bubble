#include "PropertyBoxSegment.h"
#include <iostream>
#include "Interface.h"
#include "EditorLevel.h"

PropertyBoxVertex::PropertyBoxVertex()
{
	

}

void PropertyBoxVertex::buildBox()
{
	box = sfg::Box::Create(sfg::Box::HORIZONTAL);
}

void PropertyBoxVertex::build()
{
	x = sfg::Entry::Create();
	x->SetRequisition(sf::Vector2f(30.0f,0.0f));
	x->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxVertex::set, this);
	box->Pack(x);
	
	y = sfg::Entry::Create();
	y->SetRequisition(sf::Vector2f(30.0f,0.0f));
	y->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxVertex::set, this);
	box->Pack(y);

}


void PropertyBoxVertex::update()
{
	std::stringstream ss;

	if (!x->HasFocus() && !y->HasFocus())
	{
		ss << vertex->x;
		x->SetText(ss.str());

		ss.clear();
		ss.str("");

		ss << vertex->y;
		y->SetText(ss.str());
	
	}

}



void PropertyBoxVertex::setVertex(b2Vec2* _vertex)
{
	vertex = _vertex;
}



void PropertyBoxVertex::set()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		std::string debug = x->GetText();

		ss.str(x->GetText());
		ss >> vertex->x;
		
		ss.clear();

		ss.str(y->GetText());
		ss >> vertex->y;


		box->GrabFocus();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		box->GrabFocus();
}





//Segment

PropertyBoxSegment::PropertyBoxSegment()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);
}


void PropertyBoxSegment::build(int _id)
{
	
	id = _id;

	std::stringstream ss;

	ss << "\nSegment0" << id;

	box->Pack(sfg::Label::Create(ss.str()));

	box->Pack(sfg::Separator::Create());

	sfg::Box::Ptr frictionBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		frictionBox->Pack(sfg::Label::Create("Friction:"));



		friction = sfg::Entry::Create(/*ss.str()*/);
		friction->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxSegment::setFriction, this);
		frictionBox->Pack(friction);
	}
	box->Pack(frictionBox);

	box->Pack(sfg::Separator::Create());

	sfg::Box::Ptr userDataBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		userDataBox->Pack(sfg::Label::Create("User-Data:"));

		userData = sfg::Entry::Create();
		userData->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxSegment::setUserData, this);
		userDataBox->Pack(userData);
	}
	box->Pack(userDataBox);

	sfg::Box::Ptr verticesBox;
	{
		verticesBox = sfg::Box::Create(sfg::Box::HORIZONTAL);

		verticesBox->Pack(sfg::Label::Create("Vertecies:"));
		addVertecies = sfg::Button::Create("Add Vertecies");
		addVertecies->GetSignal(sfg::Button::OnLeftClick).Connect(&PropertyBoxSegment::add, this);
		verticesBox->Pack(addVertecies);
	}
	box->Pack(verticesBox);

	sfg::Box::Ptr loopBox;
	{
		loopBox = sfg::Box::Create();

		loopBox->Pack(sfg::Label::Create("Loop:"));
		loop = sfg::ComboBox::Create();
		loop->AppendItem("false");
		loop->AppendItem("true");
		loop->GetSignal(sfg::ComboBox::OnSelect).Connect(&PropertyBoxSegment::setLoop, this);
		loopBox->Pack(loop);
	}
	//box->Pack(loopBox);
}


void PropertyBoxSegment::update()
{
	std::stringstream ss;

	ss << segment->friction;
	friction->SetText(ss.str());

	ss.clear();
	ss.str("");

	ss << *((int*)segment->userData);
	userData->SetText(ss.str());
	

	

}


void PropertyBoxSegment::setSegment(b2FixtureDef* _segment, EditorLevel* _level)
{
	segment = _segment;
	level = _level;
}

void PropertyBoxSegment::setId(int _id)
{
	id = _id;
}


void PropertyBoxSegment::setFriction()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(friction->GetText());

		ss >> segment->friction;


		box->GrabFocus();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		box->GrabFocus();

}

void PropertyBoxSegment::setUserData()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream ss;

		ss.str(userData->GetText());

		ss >> *((int*)segment->userData);


		box->GrabFocus();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		box->GrabFocus();
}


void PropertyBoxSegment::add()
{
	interface->addVertex(id);
}



void PropertyBoxSegment::setLoop()
{
	level->setLoop(id, loop->GetSelectedItem());
}


