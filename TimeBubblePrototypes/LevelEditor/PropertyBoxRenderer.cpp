#include "PropertyBoxRenderer.h"
#include "ImageManager.h"

PropertyBoxRenderer::PropertyBoxRenderer()
{
}



void PropertyBoxRenderer::setRendererComponent(SpriteComponent* _renderer)
{
	renderer = _renderer;
}


void PropertyBoxRenderer::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	sfg::Box::Ptr fileNameBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		fileNameBox->Pack(sfg::Label::Create("Filename:"));
		fileName = sfg::Entry::Create();
		fileName->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxRenderer::setFileName, this);
		fileNameBox->Pack(fileName);
	}
	box->Pack(fileNameBox, false, false);
	
	sfg::Box::Ptr originBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		originBox->Pack(sfg::Label::Create("Origin:"));

		originX = sfg::Entry::Create();
		originX->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxRenderer::setOrigin, this);
		originBox->Pack(originX);

		
		originY = sfg::Entry::Create();
		originY->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxRenderer::setOrigin, this);
		originBox->Pack(originY);


	}
	box->Pack(originBox, false, false);

	sfg::Box::Ptr scaleBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		scaleBox->Pack(sfg::Label::Create("Scale:"));

		scaleX = sfg::Entry::Create();
		scaleX->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxRenderer::setScale, this);
		scaleBox->Pack(scaleX);


		scaleY = sfg::Entry::Create();
		scaleY->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxRenderer::setScale, this);
		scaleBox->Pack(scaleY);


	}
	box->Pack(scaleBox, false, false);

}



void PropertyBoxRenderer::update()
{


	if (!fileName->HasFocus())
	{
		std::string string = imageManager->getFileName(renderer->getSprite().getTexture());
		string.erase(0,4);
		fileName->SetText(string);
	}

	if (!originX->HasFocus() && !originY->HasFocus())
	{
		std::stringstream ss;

		ss << renderer->getOrigin().x;

		originX->SetText(ss.str());

		ss.str("");
		ss.clear();

		ss << renderer->getOrigin().y;

		originY->SetText(ss.str());


	}


	if (!scaleX->HasFocus() && !scaleY->HasFocus())
	{
		std::stringstream ss;

		ss << renderer->getScale().x;

		scaleX->SetText(ss.str());

		ss.str("");
		ss.clear();

		ss << renderer->getScale().y;

		scaleY->SetText(ss.str());


	}
}


void PropertyBoxRenderer::setFileName()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		renderer->setSprite(imageManager->getTexture("res/" + fileName->GetText()));
		box->GrabFocus();
	}
}


void PropertyBoxRenderer::setOrigin()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		sf::Vector2f origin;
		std::stringstream ss;

		ss.str(originX->GetText());
		
		ss >> origin.x;

		ss.clear();
		ss.str(originY->GetText());

		ss >> origin.y;

		renderer->setOrigin(origin);

		box->GrabFocus();

	}

}

void PropertyBoxRenderer::setScale()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		sf::Vector2f scale;
		std::stringstream ss;

		ss.str(scaleX->GetText());
		
		ss >> scale.x;

		ss.clear();
		ss.str(scaleY->GetText());

		ss >> scale.y;

		renderer->setScale(scale);

		box->GrabFocus();

	}

}


// Animation





void PropertyBoxAnimation::setRendererComponent(AnimationComponent* _renderer)
{
	renderer = _renderer;
}


void PropertyBoxAnimation::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);
}

void PropertyBoxAnimation::update()
{

}



//SCML



void PropertyBoxScml::setRendererComponent(SCMLComponent* _renderer)
{
	renderer = _renderer;
}


void PropertyBoxScml::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);

	//sfg::Box::Ptr fileNameBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	//{
	//	fileNameBox->Pack(sfg::Label::Create("Filename:"));
	//	fileName = sfg::Entry::Create();
	//	fileName->SetRequisition(sf::Vector2f(200.0f, 0.0f));
	//	fileName->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxScml::setFileName, this);
	//	fileNameBox->Pack(fileName);
	//}
	//box->Pack(fileNameBox, false, false);

	box->Pack(sfg::Label::Create("Filename:"));
	fileName = sfg::Entry::Create();
	fileName->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxScml::setFileName, this);
	box->Pack(fileName);
	
	sfg::Box::Ptr scaleBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		scaleBox->Pack(sfg::Label::Create("Scale:"));

		scale = sfg::Entry::Create();
		scale->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxScml::setScale, this);
		scaleBox->Pack(scale);

		//scaleX = sfg::Entry::Create();
		//scaleX->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxScml::setScale, this);
		//scaleBox->Pack(scaleX);

		//
		//scaleY = sfg::Entry::Create();
		//scaleY->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxScml::setScale, this);
		//scaleBox->Pack(scaleY);


	}
	box->Pack(scaleBox, false, false);

	sfg::Box::Ptr orientationBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	{
		orientationBox->Pack(sfg::Label::Create("BaseOrientation:"));
		orientation = sfg::ComboBox::Create();
		orientation->AppendItem("Left");
		orientation->AppendItem("Right");
		orientation->GetSignal(sfg::ComboBox::OnSelect).Connect(&PropertyBoxScml::setOrientation, this);
		orientationBox->Pack(orientation);
	}
	box->Pack(orientationBox, false, false);

}



void PropertyBoxScml::update()
{


	if (!fileName->HasFocus())
	{
		std::string string = renderer->getFilePath();
		string.erase(0,4);
		fileName->SetText(string);
	}

	if (/*!scaleX->HasFocus() && !scaleY->HasFocus()*/!scale->HasFocus())
	{
		std::stringstream ss;

		ss << renderer->getScale().x;

		scale->SetText(ss.str());

		//scaleX->SetText(ss.str());

		//ss.str("");
		//ss.clear();

		//ss << renderer->getScale().y;

		//scaleY->SetText(ss.str());


	}

	if (!orientation->HasFocus())
	{
		orientation->SelectItem((renderer->getBaseOrientation() - 1) / 2);
	}

}


void PropertyBoxScml::setFileName()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		renderer->setScml("res/" + fileName->GetText());
		box->GrabFocus();
	}
}


void PropertyBoxScml::setScale()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{
		float _scale;
		std::stringstream ss;
		
		ss.str(scale->GetText());
		
		ss >> _scale;

		//ss.str(scaleX->GetText());
		//
		//ss >> scale.x;

		//ss.clear();
		//ss.str(scaleY->GetText());

		//ss >> scale.y;

		renderer->setScale(sf::Vector2f(_scale,_scale));

		box->GrabFocus();

	}

}


void PropertyBoxScml::setOrientation()
{
	renderer->setBaseOrientation(static_cast<Orientation>(orientation->GetSelectedItem() * 2 + 1));
}




