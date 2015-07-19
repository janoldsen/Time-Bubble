#include "PropertyBoxObject.h"
#include "LevelObject.h"
#include <SFML\Graphics.hpp>
#include <SFGUI\SFGUI.hpp>
#include "PropertyBoxAi.h"
#include "PropertyBoxRenderer.h"
#include "ShroomComponent.h"


PropertyBoxObject::~PropertyBoxObject()
{

}


void PropertyBoxObject::shutdown()
{
	if (aiBox)
	{
		delete aiBox;
		aiBox = 0;
	}
	
	if (rendererBox)
	{
		delete rendererBox;
		rendererBox = 0;

	}

	bodyBox.shutdown();


}

void PropertyBoxObject::update()
{

	if (!name->HasFocus())
		name->SetText(object->getName());
	
	std::stringstream ss;

	ss << object->getPosition().x;
	if (!positionX->HasFocus())
		positionX->SetText(ss.str());

	ss.str("");
	ss << object->getPosition().y;
	if (!positionY->HasFocus())
		positionY->SetText(ss.str());
	ss.clear();


	ss.str("");
	ss << object->getCenter().x - object->getPosition().x;
	if (!centerX->HasFocus())
		centerX->SetText(ss.str());
	ss.clear();

	ss.str("");
	ss << object->getCenter().y - object->getPosition().y;
	if (!centerY->HasFocus())
		centerY->SetText(ss.str());


	orientation->SelectItem((object->getOrientation() - 1) / 2);


	if (aiBox)
		aiBox->update();

	if (rendererBox)
		rendererBox->update();

	if (object->hasBody)
	{
		bodyBox.update();
	}


}


void PropertyBoxObject::build()
{

	box = sfg::Box::Create(sfg::Box::VERTICAL);
	box->SetSpacing(3.0f);

	sfg::Button::Ptr _export = sfg::Button::Create("Export");
	_export->GetSignal(sfg::Button::OnLeftClick).Connect(&LevelObject::exportObject, object);
	box->Pack(_export,false);


	sfg::Box::Ptr boxGeneral = sfg::Box::Create(sfg::Box::VERTICAL);
	{

		sfg::Box::Ptr nameBox = sfg::Box::Create();
		{
			nameBox->Pack(sfg::Label::Create("Name:"));

			name = sfg::Entry::Create();
			name->GetSignal(sfg::Entry::OnKeyPress).Connect(&PropertyBoxObject::setObjectName, this);
			nameBox->Pack(name);

		}
		boxGeneral->Pack(nameBox);

		sfg::Box::Ptr positionBox = sfg::Box::Create();
		{
			positionBox->Pack(sfg::Label::Create("Position: "));


			std::stringstream ss;
			ss << object->getPosition().x;
			positionX = sfg::Entry::Create(ss.str());
			positionX->SetRequisition(sf::Vector2f(20.0f, 0.0f));
			positionX->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxObject::setObjectPosition, this);
			positionBox->Pack(positionX);


			ss.str("");
			ss << object->getPosition().y;
			positionY = sfg::Entry::Create(ss.str());
			positionY->SetRequisition(sf::Vector2f(20.0f, 0.0f));
			positionY->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxObject::setObjectPosition, this);
			positionBox->Pack(positionY);
		}
		boxGeneral->Pack(positionBox, false);

		sfg::Box::Ptr centerBox = sfg::Box::Create();
		{
			centerBox->Pack(sfg::Label::Create("Center: "));


			std::stringstream ss;
			ss << object->getCenter().x;
			centerX = sfg::Entry::Create(ss.str());
			centerX->SetRequisition(sf::Vector2f(20.0f, 0.0f));
			centerX->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxObject::setObjectCenter, this);
			centerBox->Pack(centerX);


			ss.str("");
			ss << object->getCenter().y;
			centerY = sfg::Entry::Create(ss.str());
			centerY->SetRequisition(sf::Vector2f(20.0f, 0.0f));
			centerY->GetSignal(sfg::Widget::OnKeyPress).Connect(&PropertyBoxObject::setObjectCenter, this);
			centerBox->Pack(centerY);
		}
		boxGeneral->Pack(centerBox, false);

		sfg::Box::Ptr orientationBox = sfg::Box::Create();
		{
			orientationBox->Pack(sfg::Label::Create("Orientation:"));
			orientation = sfg::ComboBox::Create();
			orientation->AppendItem("Left");
			orientation->AppendItem("Right");
			orientation->GetSignal(sfg::ComboBox::OnSelect).Connect(&PropertyBoxObject::setObjectOrientation, this);

			orientationBox->Pack(orientation);


		}
		boxGeneral->Pack(orientationBox, false);

	}
	box->Pack(boxGeneral);


	box->Pack(sfg::Label::Create("AI"));
	box->Pack(sfg::Separator::Create());

	
	deleteAi = sfg::Button::Create("delete");
	deleteAi->GetSignal(sfg::Button::OnLeftClick).Connect(&PropertyBoxObject::doDeleteAi, this);
	deleteAi->Show(false);

	ais = sfg::ComboBox::Create();

	ais->AppendItem("Plattform");
	ais->AppendItem("Swing");
	ais->AppendItem("Bubble");
	ais->AppendItem("Jumper");
	ais->AppendItem("Player");
	ais->AppendItem("Enemy");
	ais->AppendItem("Event");
	ais->AppendItem("Shroom");
	ais->AppendItem("Particle");
	ais->AppendItem("Bomb");
	ais->AppendItem("");

	aBox = sfg::Box::Create(sfg::Box::VERTICAL);

	aBox->Pack(deleteAi);

	ais->GetSignal(sfg::ComboBox::OnSelect).Connect(&PropertyBoxObject::addAi, this);
	aBox->Pack(ais);


	if (object->getAiComponent())
	{
		addAi(object->getAiComponent()->getType());
	}

	box->Pack(aBox);


	box->Pack(sfg::Label::Create("RENDERER"));
	box->Pack(sfg::Separator::Create());


	deleteRenderer = sfg::Button::Create("delete");
	deleteRenderer->GetSignal(sfg::Button::OnLeftClick).Connect(&PropertyBoxObject::doDeleteRenderer, this);
	deleteRenderer->Show(false);

	renderers = sfg::ComboBox::Create();

	renderers->AppendItem("Renderer");
	renderers->AppendItem("Animation");
	renderers->AppendItem("");


	rBox = sfg::Box::Create(sfg::Box::VERTICAL);

	rBox->Pack(deleteRenderer);

	renderers->GetSignal(sfg::ComboBox::OnSelect).Connect(&PropertyBoxObject::addRenderer, this);

	rBox->Pack(renderers);

	if (object->getRendererComponent())
	{
		addRenderer(object->getRendererComponent()->getType());
	}


	box->Pack(rBox);


	box->Pack(sfg::Label::Create("BODY"));
	box->Pack(sfg::Separator::Create());

	bBox = sfg::Box::Create(sfg::Box::VERTICAL);

	deleteBody = sfg::Button::Create("delete");
	deleteBody->GetSignal(sfg::Button::OnLeftClick).Connect(&PropertyBoxObject::doDeleteBody, this);
	deleteBody->Show(false);

	bBox->Pack(deleteBody);

	addBody = sfg::Button::Create("Add Body");
	addBody->GetSignal(sfg::Button::OnLeftClick).Connect(&PropertyBoxObject::doAddBody, this);
	bBox->Pack(addBody);

	if (object->hasBody)
	{
		doAddBody();

	}
	else
	{
		addBody->Show(true);
	}


	box->Pack(bBox);

}

void PropertyBoxObject::setObject(LevelObject* _object)
{
	object = _object;
}

void PropertyBoxObject::setObjectCenter()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{

		sf::Vector2f center;

		std::stringstream ssX;
		ssX.str(centerX->GetText());
		ssX >> center.x;
		
		std::stringstream ssY;
		ssY.str(centerY->GetText());
		ssY >> center.y;
		object->setCenter(center);

		box->GrabFocus();

	}
}

void PropertyBoxObject::setObjectPosition()
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

void PropertyBoxObject::setObjectName()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	{

		object->setName(name->GetText());

		box->GrabFocus();

	}
}

void PropertyBoxObject::setObjectOrientation()
{
	object->setOrientation(static_cast<Orientation>(orientation->GetSelectedItem() * 2 + 1));
}

void PropertyBoxObject::addAi()
{

	if (ais->GetSelectedText() == "Plattform")
	{
		addAi(ComponentTypes::PLATTFORM);
		return;
	}
	else if (ais->GetSelectedText() == "Swing")
	{
		addAi(ComponentTypes::SWING);
		return;
	}
	else if (ais->GetSelectedText() == "Bubble")
	{
		addAi(ComponentTypes::BUBBLE);
		return;
	}
	else if (ais->GetSelectedText() == "Jumper")
	{
		addAi(ComponentTypes::JUMPER);
		return;
	}
	else if (ais->GetSelectedText() == "Player")
	{
		addAi(ComponentTypes::PLAYER);
		return;
	}
	else if (ais->GetSelectedText() == "Event")
	{
		addAi(ComponentTypes::EVENT);
	}
	else if (ais->GetSelectedText() == "Enemy")
	{
		addAi(ComponentTypes::ENEMY);
	}
	else if (ais->GetSelectedText() == "Shroom")
	{
		addAi(ComponentTypes::SHROOM);
	}
	else if (ais->GetSelectedText() == "Particle")
	{
		addAi(ComponentTypes::PARTICLE);
	}
	else if (ais->GetSelectedText() == "Bomb")
	{
		addAi(ComponentTypes::BOMB);
	}
}

void PropertyBoxObject::addAi(ComponentTypes type)
{
	ais->Show(false);
	ais->SelectItem(ais->GetItemCount() - 1);


	switch (type)
	{
	case ComponentTypes::PLATTFORM:
		{
			PropertyBoxPlattform* _aiBox  = new PropertyBoxPlattform();
			_aiBox->setObject(object);

			if (object->getAiComponent())
			{
				_aiBox->setPlattformComponent(dynamic_cast<PlattformComponent*> (object->getAiComponent()));
			}
			else
			{
				PlattformComponent* ai = new PlattformComponent();
				object->setAiComponent(ai);
				ai->setParent(object);
				_aiBox->setPlattformComponent(ai);

			}

			aiBox =_aiBox;
		}
		break;
	case ComponentTypes::SWING:
		{
			PropertyBoxSwing* _aiBox = new PropertyBoxSwing();

			if (object->getAiComponent())
			{
				_aiBox->setSwingComponent(dynamic_cast<SwingComponent*> (object->getAiComponent()));
			}
			else
			{
				SwingComponent* ai = new SwingComponent();
				object->setAiComponent(ai);
				ai->setParent(object);
				_aiBox->setSwingComponent(ai);

			}

			aiBox = _aiBox;
		}
		break;
	case ComponentTypes::BUBBLE:
		{
			PropertyBoxBubble* _aiBox = new PropertyBoxBubble();

			if (object->getAiComponent())
			{
				_aiBox->setBubbleComponent(dynamic_cast<TimeBubbleComponent*>(object->getAiComponent()));
			}
			else
			{
				TimeBubbleComponent* ai = new TimeBubbleComponent();
				object->setAiComponent(ai);
				ai->setParent(object);
				_aiBox->setBubbleComponent(ai);

			}

			aiBox = _aiBox;
		}
		break;
	case ComponentTypes::JUMPER:
		{
			PropertyBoxJumper* _aiBox = new PropertyBoxJumper();

			if (object->getAiComponent())
			{
				_aiBox->setJumperComponent(dynamic_cast<JumperComponent*>(object->getAiComponent()));
			}
			else
			{
				JumperComponent* ai = new JumperComponent();
				object->setAiComponent(ai);
				ai->setParent(object);
				_aiBox->setJumperComponent(ai);
			}
			aiBox = _aiBox;
		}
		break;
	case ComponentTypes::PLAYER:
		{
			PropertyBoxPlayer* _aiBox = new PropertyBoxPlayer();

			if (object->getAiComponent())
			{
				_aiBox->setPlayerComponent(dynamic_cast<PlayerComponent*>(object->getAiComponent()));
			}
			else
			{
				PlayerComponent* ai = new PlayerComponent();
				object->setAiComponent(ai);
				ai->setParent(object);
				_aiBox->setPlayerComponent(ai);

			}
			aiBox = _aiBox;

		}
		break;
	case ComponentTypes::EVENT:
		{
			PropertyBoxEvent* _aiBox = new PropertyBoxEvent();

			if (object->getAiComponent())
			{
				_aiBox->setEventComponent(dynamic_cast<EventComponent*>(object->getAiComponent()));
			}
			else
			{
				EventComponent* ai = new EventComponent();
				object->setAiComponent(ai);
				ai->setParent(object);
				_aiBox->setEventComponent(ai);

			}
			aiBox = _aiBox; 
		}
		break;
	case ComponentTypes::ENEMY:
		{
			PropertyBoxEnemy* _aiBox = new PropertyBoxEnemy();

			if (object->getAiComponent())
			{
				_aiBox->setEnemyComponent(dynamic_cast<EnemyComponent*>(object->getAiComponent()));
			}
			else
			{
				EnemyComponent* ai = new EnemyComponent();
				object->setAiComponent(ai);
				ai->setParent(object);
				_aiBox->setEnemyComponent(ai);
			}
			aiBox = _aiBox;
		}
		break;
	case ComponentTypes::SHROOM:
		{
			PropertyBoxShroom* _aiBox = new PropertyBoxShroom();

			if (object->getAiComponent())
			{
				_aiBox->setShroomComponent(dynamic_cast<ShroomComponent*>(object->getAiComponent()));
			}
			else
			{
				ShroomComponent* ai = new ShroomComponent();
				object->setAiComponent(ai);
				ai->setParent(object);
				_aiBox->setShroomComponent(ai);
			}
			aiBox = _aiBox;
		}
		break;

	case ComponentTypes::PARTICLE:
		{
			PropertyBoxParticle* _aiBox = new PropertyBoxParticle();

			if (object->getAiComponent())
			{
				_aiBox->setParticleComponent(dynamic_cast<ParticleComponent*>(object->getAiComponent()));
			}
			else
			{
				ParticleComponent* ai = new ParticleComponent();
				object->setAiComponent(ai);
				ai->setParent(object);
				_aiBox->setParticleComponent(ai);
			}
			aiBox = _aiBox;
		}
		break;
	case ComponentTypes::ROCK:
		{
			PropertyBoxRock* _aiBox = new PropertyBoxRock();

			if (object->getAiComponent())
			{
				_aiBox->setRockComponent(dynamic_cast<RockComponent*>(object->getAiComponent()));
			}
			else
			{
				RockComponent* ai = new RockComponent();
				object->setAiComponent(ai);
				ai->setParent(object);
				_aiBox->setRockComponent(ai);
			}
			aiBox = _aiBox;
		}
		break;
	case ComponentTypes::BOMB:
		{
			PropertyBoxBomb* _aiBox = new PropertyBoxBomb();

			if (object->getAiComponent())
			{
				_aiBox->setBombComponent(dynamic_cast<BombComponent*>(object->getAiComponent()));
			}
			else
			{
				BombComponent* ai = new BombComponent();
				object->setAiComponent(ai);
				ai->setParent(object);
				_aiBox->setBombComponent(ai);
			}
			aiBox = _aiBox;
		}
		break;

	default:
		aiBox = new PropertyBoxAi();
		break;

	}


	deleteAi->Show(true);

	aiBox->build();

	aBox->Pack(aiBox->get());


}


void PropertyBoxObject::doDeleteAi()
{

	delete object->getAiComponent();

	object->setAiComponent(0);


	aiBox->get()->Show(false);

	delete aiBox;
	aiBox = 0;

	deleteAi->Show(false);

	ais->Show(true);

}

void PropertyBoxObject::addRenderer()
{

	if (renderers->GetSelectedText() == "Renderer")
	{
		addRenderer(ComponentTypes::STANDARD);
		return;
	}

	if (renderers->GetSelectedText() == "Animation")
	{
		addRenderer(ComponentTypes::SCML);
		return;
	}



}



void PropertyBoxObject::addRenderer(ComponentTypes type)
{

	renderers->Show(false);
	renderers->SelectItem(2);

	switch (type)
	{
	case ComponentTypes::STANDARD:
		{
			PropertyBoxRenderer* _rendererBox = new PropertyBoxRenderer();

			if (object->getRendererComponent())
			{
				_rendererBox->setRendererComponent(dynamic_cast<SpriteComponent*>(object->getRendererComponent()));
			}
			else
			{
				SpriteComponent* renderer = new SpriteComponent();
				object->setRendererComponent(renderer);
				_rendererBox->setRendererComponent(renderer);

			}
			
			rendererBox = _rendererBox;
		}
		break;
	case ComponentTypes::SCML:
		{
			PropertyBoxScml* _rendererBox = new PropertyBoxScml();

			if (object->getRendererComponent())
			{
				_rendererBox->setRendererComponent(dynamic_cast<SCMLComponent*>(object->getRendererComponent()));
			}
			else
			{
				SCMLComponent* renderer = new SCMLComponent();
				object->setRendererComponent(renderer);
				_rendererBox->setRendererComponent(renderer);

			}

			rendererBox = _rendererBox;

		}
		break;
	case ComponentTypes::ANIMATION:
		{
			PropertyBoxAnimation* _rendererBox = new PropertyBoxAnimation();
		
			if (object->getRendererComponent())
			{
				_rendererBox->setRendererComponent(dynamic_cast<AnimationComponent*>(object->getRendererComponent()));
			}
			else
			{
				AnimationComponent* renderer = new AnimationComponent();
				object->setRendererComponent(renderer);
				_rendererBox->setRendererComponent(renderer);

			}

			rendererBox = _rendererBox;
		}
		break;

	}


	rendererBox->build();

	//rBox->Pack(deleteRenderer);

	deleteRenderer->Show(true);

	rBox->Pack(rendererBox->get());



}

void PropertyBoxObject::doDeleteRenderer()
{

	delete object->getRendererComponent();

	object->setRendererComponent(0);


	rendererBox->get()->Show(false);

	delete rendererBox;
	rendererBox = 0;

	deleteRenderer->Show(false);

	renderers->Show(true);

}



void PropertyBoxObject::doAddBody()
{
	addBody->Show(false);

	object->hasBody = true;

	bodyBox.setObject(object);
	bodyBox.build();

	bBox->Pack(bodyBox.get());

	deleteBody->Show(true);

}

void PropertyBoxObject::doDeleteBody()
{
	object->hasBody = false;

	bodyBox.shutdown();

	for (int i = 0; i < object->shapes.size(); i++)
		delete object->shapes[i];
	object->shapes.clear();

	for (int i = 0; i < object->fixtureDefs.size(); i++)
		delete object->fixtureDefs[i].userData;
	object->fixtureDefs.clear();

	addBody->Show(true);


}