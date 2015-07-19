#include "PropertyBoxLevel.h"
#include <ImageManager.h>
#include "EditorLevel.h"
#include "Interface.h"



PropertyBoxLevel::~PropertyBoxLevel()
{
	for (int i = 0; i < segments.size(); i++)
	{
		delete segments[i];
	}

}


void PropertyBoxLevel::shutdown()
{
	for (int i = 0; i < segments.size(); i++)
	{
		delete segments[i];
	}
	segments.clear();

}


void PropertyBoxLevel::setLevel(EditorLevel& _level)
{
	level = &_level;

	std::string bgString = imageManager->getFileName(level->getBackGround());
	bgString.erase(0,4);
	backGround->SetText(bgString);

	std::string ggString = imageManager->getFileName(level->getGameGround());
	ggString.erase(0,4);
	gameGround->SetText(ggString);


	std::stringstream left;
	left << level->getSize().left;
	sizeLeft->SetText(left.str());
	std::stringstream top;
	top << level->getSize().top;
	sizeTop->SetText(top.str());
	std::stringstream width;
	width << level->getSize().width;
	sizeWidth->SetText(width.str());
	std::stringstream height;
	height << level->getSize().height;
	sizeHeight->SetText(height.str());



}


void PropertyBoxLevel::update()
{
	std::string bgString = imageManager->getFileName(level->getBackGround());
	bgString.erase(0,4);
	backGround->SetText(bgString);

	std::string ggString = imageManager->getFileName(level->getGameGround());
	ggString.erase(0,4);
	gameGround->SetText(ggString);


	std::stringstream left;
	left << level->getSize().left;
	sizeLeft->SetText(left.str());
	std::stringstream top;
	top << level->getSize().top;
	sizeTop->SetText(top.str());
	std::stringstream width;
	width << level->getSize().width;
	sizeWidth->SetText(width.str());
	std::stringstream height;
	height << level->getSize().height;
	sizeHeight->SetText(height.str());

	for (int i = 0; i < segments.size(); i++)
	{
		segments[i]->update();
	}

}



void PropertyBoxLevel::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);
	

	Box bgBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	bgBox->Pack(sfg::Label::Create("Background:"),false);
	backGround = sfg::Entry::Create();
	backGround->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxLevel::setBackGround, this);
	backGround->SetRequisition(sf::Vector2f(150.0f,0.0f));
	bgBox->Pack(backGround);
	box->Pack(bgBox);

	Box ggBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	ggBox->Pack(sfg::Label::Create("Gameground:"));
	gameGround = sfg::Entry::Create();
	gameGround->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxLevel::setGameGround, this);
	gameGround->SetRequisition(sf::Vector2f(150.0f,0.0f));
	ggBox->Pack(gameGround);
	box->Pack(ggBox);

	box->Pack(sfg::Separator::Create());


	box->Pack(sfg::Label::Create("Size:"), false, false);


	Box sizeBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	sizeLeft = sfg::Entry::Create();
	sizeTop = sfg::Entry::Create();
	sizeWidth = sfg::Entry::Create();
	sizeHeight = sfg::Entry::Create();
	
	sizeLeft->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxLevel::setSize, this);
	sizeTop->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxLevel::setSize, this);
	sizeWidth->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxLevel::setSize, this);
	sizeHeight->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxLevel::setSize, this);
	
	sizeLeft->SetRequisition(sf::Vector2f(30.0f, 0.0f));
	sizeTop->SetRequisition(sf::Vector2f(30.0f, 0.0f));
	sizeWidth->SetRequisition(sf::Vector2f(30.0f, 0.0f));
	sizeHeight->SetRequisition(sf::Vector2f(30.0f, 0.0f));
	
	
	sizeBox->Pack(sizeLeft,false);
	sizeBox->Pack(sizeTop,false);
	sizeBox->Pack(sizeWidth,false);
	sizeBox->Pack(sizeHeight,false);
	box->Pack(sizeBox);

	box->Pack(sfg::Separator::Create());

	sfg::Box::Ptr segmentsBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	segmentsBox->Pack(sfg::Label::Create("Segments:"));
	addSegmentButton = sfg::Button::Create("Add Segment");
	addSegmentButton->GetSignal(sfg::Button::OnLeftClick).Connect(&PropertyBoxLevel::_addSegment, this);
	segmentsBox->Pack(addSegmentButton);
	box->Pack(segmentsBox);


}


void PropertyBoxLevel::addSegment(b2FixtureDef* _segment)
{
	segments.push_back(new PropertyBoxSegment());

	segments.back()->setSegment(level->fixtureDefs.back(), level);
	segments.back()->build(segments.size()-1);
	segments.back()->update();

	box->Pack(segments.back()->get());

}

void PropertyBoxLevel::deleteSegment(int segmentId)
{
	segments[segmentId]->get()->RemoveAll();

	box->Remove(segments[segmentId]->get());
	box->RefreshAll();


	for (int i = segmentId + 1; i < segments.size(); i++)
	{
		segments[i]->setId(i - 1);
	}

	delete segments[segmentId];
	segments.erase(segments.begin() + segmentId);
}


void PropertyBoxLevel::setBackGround()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::string bgString = backGround->GetText();
		level->setBackGround(imageManager->getTexture("res/" + bgString));
		box->GrabFocus();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		box->GrabFocus();

}


void PropertyBoxLevel::setGameGround()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::string ggString = gameGround->GetText();
		level->setGameGround(imageManager->getTexture("res/" + ggString));
		box->GrabFocus();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		box->GrabFocus();

}

void PropertyBoxLevel::setSize()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		std::stringstream sizeString;
		sf::IntRect size;
		sizeString.str(sizeLeft->GetText());
		sizeString >> size.left;
		sizeString.clear();
		sizeString.str(sizeTop->GetText());
		sizeString >> size.top;
		sizeString.clear();
		sizeString.str(sizeWidth->GetText());
		sizeString >> size.width;
		sizeString.clear();
		sizeString.str(sizeHeight->GetText());
		sizeString >> size.height;

		level->setSize(size);

		box->GrabFocus();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		box->GrabFocus();
}


void PropertyBoxLevel::_addSegment()
{
	interface->addSegment();
}




