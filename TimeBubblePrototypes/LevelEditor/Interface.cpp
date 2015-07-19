#include "Interface.h"
#include "Defines.h"
#include <myMath.h>
#include "dirent.h"
#include <iostream>


Interface Interface::instance;


Interface::Interface()
{
	state = IDLE;
	movingView = false;




}



Interface* Interface::getInstance()
{
	return &instance;
}

void Interface::initialize()
{
	window = sfg::Window::Create();
	entry = sfg::Entry::Create();
}


void Interface::initEditor(LevelEditor* _editor, EditorLevel* _level, ObjectContainer* _gameObjects, sfg::Desktop* _desktop)
{
	editor = _editor;
	level = _level;
	gameObjects = _gameObjects;
	desktop = _desktop;
}

void Interface::initView(sf::RenderWindow* _mainWindow, sfg::Canvas::Ptr* _viewCanvas, sf::RenderTarget* _viewPort, Camera* _camera)
{
	mainWindow = _mainWindow;
	viewCanvas = _viewCanvas;
	viewPort = _viewPort;
	camera = _camera;

}

void Interface::initSelector(Selector* _selector)
{
	selector = _selector;
}


void Interface::onLeftPress()
{

	(*viewCanvas)->GrabFocus();

	position = viewPort->mapPixelToCoords(sf::Mouse::getPosition(*mainWindow) - (sf::Vector2i)(*viewCanvas)->GetAbsolutePosition(), camera->getView());


	position = MATH::vPxtoM(position);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		movingView = true;
		startPosition = position;
	}

	switch (state)
	{
	case IDLE:
		{
			//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			//{
			//	state = MOVINGVIEW;
			//	startPosition = position;
			//}
			//else
			if (!movingView)
			{
				if (selector->selection.size() > 0)
				{
					sf::Vector2f _position = MATH::vMtoPx(position);

					if (selector->handle.contains(_position))
					{
						state = SELECTIONTRANSLATION;
						selector->translating = true;
					}
					else if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
					{
						selector->clearSelection();
					}
				}

				if (!selector->selecting && state == IDLE)
				{
					selector->selecting = true;

					state = SELECTING;

					selector->selectionRect.left = position.x;
					selector->selectionRect.top = position.y;

					selector->selectionRect.width = 0;
					selector->selectionRect.height = 0;
				}
			}

		}
		break;
	case WINDOWISOPEN:
		{
			desktop->BringToFront(window);
		}
		break;

	}
}

void Interface::onRightClick()
{

	(*viewCanvas)->GrabFocus();

	switch (state)
	{
	case ADDINGVERTEX:
		state = IDLE;
		break;
	case WINDOWISOPEN:
		{
			desktop->Remove(window);
			window->RemoveAll();
			entry = sfg::Entry::Create();
			window = sfg::Window::Create();
			state = IDLE;
		}
		break;
	case ADDINGBGOBJECT:
		state = IDLE;
		break;
	}
}

void Interface::onLeftRelease()
{
	if (movingView)
		movingView = false;
	else
	{

		switch (state)
		{
		case IDLE:
			{
			}
			break;
		case SELECTING:
			{
				selector->selecting = false;
				selector->checkSelection(*level, gameObjects->begin);
				state = IDLE;
			}
			break;
		case SELECTIONTRANSLATION:
			{
				state = IDLE;
			}
			break;
			//case MOVINGVIEW:
			//	{
			//		state = IDLE;
			//	}
			//	break;
		case ADDINGVERTEX:
			{
				level->addVertex(*selector, segmentId, MATH::sfTob2(position));
			}
			break;
		case SETTINGVERTEX:
			{
				vertecies[count++] = MATH::sfTob2(position);
				if (count > 1)
				{
					level->setVertices(*selector, segmentId, vertecies, 2);
					state = ADDINGVERTEX;
				}

			}
			break;
		case IMPORTINGOBJECT:
			{
				tempObject->setPosition(position);
				editor->addObject(tempObject);

				tempObject = 0;
				state = IDLE;

			}
			break;
		case ADDINGBGOBJECT:
			{
				BackGroundObject* object = new BackGroundObject();
				object->setName("New");
				object->setPosition(position);
				object->setDepth(1.0f);

				editor->addBackGroundObject(object);
				
				state = IDLE;
			}

		}
	}
}




void Interface::onMouseMove()
{
	sf::Vector2f newPosition = viewPort->mapPixelToCoords(sf::Mouse::getPosition(*mainWindow) - (sf::Vector2i)(*viewCanvas)->GetAbsolutePosition(), camera->getView());
	newPosition = MATH::vPxtoM(newPosition);



	if (movingView)
	{
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			movingView = false;

		sf::Vector2f move = (startPosition - newPosition);
		move = MATH::vMtoPx(move);
		sf::View view = camera->getView();
		view.move(move);
		camera->setView(view);

	}
	else
	{

		switch (state)
		{
		case IDLE:
			{

			}
			break;
		case SELECTIONTRANSLATION:
			{

				sf::Vector2f move = newPosition - position;
				selector->moveSelection(move);

			}
			break;
		case SELECTING:
			{
				selector->selectionRect.width = newPosition.x - selector->selectionRect.left;
				selector->selectionRect.height = newPosition.y - selector->selectionRect.top;
			}
			break;
			//case MOVINGVIEW:
			//	{
			//		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
			//			state = IDLE;

			//		sf::Vector2f move = (startPosition - newPosition);
			//		move = MATH::vMtoPx(move);
			//		view->move(move);
			//	}
			//	break;
		}
	}
	position = newPosition;
}

void Interface::onKeyPress()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Delete))
	{
		selector->deleteSelection();
	}

}



void Interface::addVertex(int _segmentId)
{
	if (state == IDLE)

	{
		segmentId = _segmentId;
		state = ADDINGVERTEX;
	}
}



void Interface::addSegment()
{
	b2FixtureDef fixtureDef;
	fixtureDef.userData = new int(GROUND);

	segmentId = level->addSegment(fixtureDef);

	state = SETTINGVERTEX;
	count = 0;
}


void Interface::newObject()
{
	tempObject = new LevelObject();
	tempObject->setName("New");

	state = IMPORTINGOBJECT;

}

void Interface::importObject()
{
	DIR* dir = opendir("assets");

	if (dir == NULL)
	{
		std::cout << "error" << std::endl;

		return;
	}


	window->SetPosition(sf::Vector2f(400.0f, 300.0f));

	sfg::Box::Ptr box = sfg::Box::Create(sfg::Box::VERTICAL);

	while (dirent* ent = readdir(dir))
	{
		std::string name(ent->d_name);

		if (name.size() >= 4)
		{
			if (std::string(name, name.length()-3, 3) == "ass")
			{
				sfg::Label::Ptr label = sfg::Label::Create(name);
				label->SetAlignment(sf::Vector2f(0.0f, 0.0f));
				label->GetSignal(sfg::Widget::OnLeftClick).Connect(&Interface::doImportObject, this);
				box->Pack(label,false);
			}
		}

	}
	window->Add(box);
	desktop->Add(window);

	state = WINDOWISOPEN;

	closedir(dir);

}

void Interface::doImportObject()
{
	sfg::Widget::Ptr widget = sfg::Context::Get().GetActiveWidget();


	sfg::Label::Ptr label = sfg::DynamicPointerCast<sfg::Label, sfg::Widget>(widget);

	tempObject = new LevelObject();

	tempObject->importObject(label->GetText());

	state = IMPORTINGOBJECT;
	

	desktop->Remove(window);
	
	window = sfg::Window::Create();

}



void Interface::addBgObject()
{
	state = ADDINGBGOBJECT;
}


void Interface::exportLevel()
{
	window->SetPosition(sf::Vector2f(400,300));

	sfg::Box::Ptr box = sfg::Box::Create(sfg::Box::HORIZONTAL);

	entry->SetRequisition(sf::Vector2f(100.0f,0.0f));

	box->Pack(entry);

	sfg::Button::Ptr button = sfg::Button::Create("Export");
	button->GetSignal(sfg::Button::OnLeftClick).Connect(&Interface::doExportLevel, this);
	box->Pack(button);

	window->Add(box);

	desktop->Add(window);

	state = WINDOWISOPEN;

}

void Interface::doExportLevel()
{
	editor->exportPng(entry->GetText());

	desktop->Remove(window);

	window = sfg::Window::Create();

	entry = sfg::Entry::Create();

	state = IDLE;


}

void Interface::saveLevel()
{
	window->SetPosition(sf::Vector2f(400,300));

	sfg::Box::Ptr box = sfg::Box::Create(sfg::Box::HORIZONTAL);

	entry->SetRequisition(sf::Vector2f(100.0f,0.0f));

	box->Pack(entry);

	sfg::Button::Ptr button = sfg::Button::Create("Save");
	button->GetSignal(sfg::Button::OnLeftClick).Connect(&Interface::doSaveLevel, this);
	box->Pack(button);

	window->Add(box);

	desktop->Add(window);

	state = WINDOWISOPEN;


}

void Interface::doSaveLevel()
{
	editor->save(entry->GetText());

	desktop->Remove(window);

	window = sfg::Window::Create();

	entry = sfg::Entry::Create();

	state = IDLE;
}

void Interface::loadLevel()
{
	DIR* dir = opendir("Levels");

	if (dir == NULL)
	{
		std::cout << "error" << std::endl;

		return;
	}


	window->SetPosition(sf::Vector2f(400.0f, 300.0f));

	sfg::Box::Ptr box = sfg::Box::Create(sfg::Box::VERTICAL);

	while (dirent* ent = readdir(dir))
	{
		std::string name(ent->d_name);

		if (name.size() >= 4)
		{
			if (std::string(name, name.length()-3, 3) == "lvl")
			{
				sfg::Label::Ptr label = sfg::Label::Create(name);
				label->SetAlignment(sf::Vector2f(0.0f, 0.0f));
				label->GetSignal(sfg::Widget::OnLeftClick).Connect(&Interface::doLoadLevel, this);
				box->Pack(label,false);
			}
		}

	}
	window->Add(box);
	desktop->Add(window);

	state = WINDOWISOPEN;

	closedir(dir);
}

void Interface::doLoadLevel()
{
	sfg::Widget::Ptr widget = sfg::Context::Get().GetActiveWidget();


	sfg::Label::Ptr label = sfg::DynamicPointerCast<sfg::Label, sfg::Widget>(widget);

	editor->shutdown();
	editor->initialize();
	editor->load(label->GetText());

	state = IDLE;
	

	desktop->Remove(window);
	
	window = sfg::Window::Create();

}