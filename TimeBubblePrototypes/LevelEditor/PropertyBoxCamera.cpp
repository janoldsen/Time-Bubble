#include "PropertyBoxCamera.h"
#include "Camera.h"
#include "Level.h"
#include <Defines.h>



void PropertyBoxCamera::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	sfg::Box::Ptr zoomBox = sfg::Box::Create();
	{
		zoomBox->Pack(sfg::Label::Create("Zoom:"));

		zoom = sfg::Entry::Create();
		zoom->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxCamera::setZoom, this);
		zoomBox->Pack(zoom);

	}
	box->Pack(zoomBox);

	box->Pack(sfg::Label::Create("Position:"));
	sfg::Adjustment::Ptr adjustment;
	sfg::Box::Ptr positionBoxX = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		
		
		positionX = sfg::Scrollbar::Create(sfg::Range::HORIZONTAL);
		adjustment = positionX->GetAdjustment();
		adjustment->SetLower(20);
		adjustment->SetUpper(80);
		adjustment->SetMajorStep(5);
		adjustment->SetMinorStep(1);
		adjustment->GetSignal(sfg::Adjustment::OnChange).Connect(&PropertyBoxCamera::setPosition, this);
		positionX->SetRequisition(sf::Vector2f(150.0f, 0.0f));
		positionBoxX->Pack(positionX,false,false);
	}
	box->Pack(positionBoxX, false, false);

	sfg::Box::Ptr positionBoxY = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		positionY = sfg::Scrollbar::Create(sfg::Range::VERTICAL);
		adjustment = positionY->GetAdjustment();
		adjustment->SetLower(20);
		adjustment->SetUpper(80);
		adjustment->SetMajorStep(5);
		adjustment->SetMinorStep(1);
		adjustment->GetSignal(sfg::Adjustment::OnChange).Connect(&PropertyBoxCamera::setPosition, this);
		positionY->SetRequisition(sf::Vector2f(0.0f, 150.0f));
		positionBoxY->Pack(positionY,false,false);


	}
	box->Pack(positionBoxY, false, false);



	sfg::Box::Ptr colorBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		colorBox->Pack(sfg::Label::Create("Fog Color:"));

		
		colorR = sfg::Entry::Create("0.4");
		colorR->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxCamera::setColor, this);
		colorBox->Pack(colorR);
		colorG = sfg::Entry::Create("0.5");
		colorBox->Pack(colorG);
		colorG->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxCamera::setColor, this);
		colorB = sfg::Entry::Create("0.6");
		colorBox->Pack(colorB);
		colorB->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxCamera::setColor, this);

	}
	box->Pack(colorBox);

	sfg::Box::Ptr strengthBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		strengthBox->Pack(sfg::Label::Create("Fog Strength:"));
		strength = sfg::Entry::Create("0.03");
		strength->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxCamera::setStrength, this);
		strengthBox->Pack(strength);
	}
	box->Pack(strengthBox);

	clouds = sfg::CheckButton::Create("Clouds enabled");
	clouds->SetActive(false);
	clouds->GetSignal(sfg::CheckButton::OnToggle).Connect(&PropertyBoxCamera::setClouds, this);
	box->Pack(clouds, false, false);


	depthEnabled = sfg::CheckButton::Create("Enable Depth");
	depthEnabled->SetActive(true);
	depthEnabled->GetSignal(sfg::CheckButton::OnToggle).Connect(&PropertyBoxCamera::toggleDepth, this);
	box->Pack(depthEnabled, false, false);

	sfg::Box::Ptr sunStrengthBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		sunStrengthBox->Pack(sfg::Label::Create("Sun Strength:"));
		sunStrength = sfg::Entry::Create("0.03");
		sunStrength->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxCamera::setSunStrength, this);
		sunStrengthBox->Pack(sunStrength);
	}
	box->Pack(sunStrengthBox);

}


void PropertyBoxCamera::update()
{

	if (!zoom->HasFocus())
	{
		std::stringstream ss;

		ss << camera->getZoom();

		zoom->SetText(ss.str());
	}

	positionX->SetRange(level->getSize().left + 8.0f * camera->getZoom(), level->getSize().left + level->getSize().width - 8.0f * camera->getZoom());
	positionY->SetRange(-level->getSize().top + 6.0f * camera->getZoom(), -level->getSize().top + level->getSize().height- 6.0f * camera->getZoom());


	if (!colorR->HasFocus() && !colorG->HasFocus() && !colorB->HasFocus())
	{
		std::stringstream ss;

		ss << camera->getFogColor().x;
		colorR->SetText(ss.str());
		ss.str("");
		ss.clear();

		ss << camera->getFogColor().y;
		colorG->SetText(ss.str());
		ss.str("");
		ss.clear();

		ss << camera->getFogColor().z;
		colorB->SetText(ss.str());
		ss.str("");
		ss.clear();
	}

	if (!strength->HasFocus())
	{
		std::stringstream ss;

		ss << camera->getFogStrength();

		strength->SetText(ss.str());
	}


	if (!sunStrength->HasFocus())
	{
		std::stringstream ss;

		ss << camera->getSunStrength();

		sunStrength->SetText(ss.str());
	}

	//clouds->SetActive(camera->getClouds());

}

void PropertyBoxCamera::setCamera(Camera* _camera)
{
	camera = _camera;
}

void PropertyBoxCamera::setLevel(Level* _level)
{
	level = _level;
	positionX->SetRange(level->getSize().left + 16, level->getSize().left + level->getSize().width - 16);
	positionY->SetRange(level->getSize().top - level->getSize().height + 12, level->getSize().top - 12);

}



void PropertyBoxCamera::setZoom()
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		float _zoom = atof(zoom->GetText().toAnsiString().c_str());

		camera->setZoom(_zoom);

		box->GrabFocus();
	}
}


void PropertyBoxCamera::setPosition()
{
	sf::Vector2f position;
	position.x = (positionX->GetValue()) * mToPx ;
	position.y = (positionY->GetValue()) * mToPx ;

	camera->setPosition(position);
}


void PropertyBoxCamera::toggleDepth()
{
	camera->setDepthEnabled(!camera->isDepthEnabled());
}



void PropertyBoxCamera::setColor()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		sf::Vector3f color;

		color.x = atof(colorR->GetText().toAnsiString().c_str());
		color.y = atof(colorG->GetText().toAnsiString().c_str());
		color.z = atof(colorB->GetText().toAnsiString().c_str());

		camera->setFogColor(color);

		box->GrabFocus();
	}

}


void PropertyBoxCamera::setStrength()
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		float _strength = atof(strength->GetText().toAnsiString().c_str());

		camera->setFogStrength(_strength);

		box->GrabFocus();
	}
}



void PropertyBoxCamera::setClouds()
{
	camera->setClouds(!camera->getClouds());
}



void PropertyBoxCamera::setSunStrength()
{

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		float _sunStrength = atof(sunStrength->GetText().toAnsiString().c_str());

		camera->setSunStrength(_sunStrength);

		box->GrabFocus();
	}
}