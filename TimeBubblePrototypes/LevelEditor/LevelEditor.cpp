#include "LevelEditor.h"
#include <fstream>
#include <ObjectIterator.h>
#include <AnimationComponent.h>
#include <PlayerComponent.h>
#include <PlattformComponent.h>
#include <iostream>
#include <sstream>
#include <myMath.h>
#include "Interface.h"
#include <SwingComponent.h>
#include <TimeBubbleComponent.h>
#include <JumperComponent.h>
#include <GameClock.h>
#include <EventComponent.h>
#include <SCMLComponent.h>
#include <EnemyComponent.h>
#include <ParticleComponent.h>
#include "RockComponent.h"
#include <BombComponent.h>

#include <random>



typedef sfg::Button::Ptr Button;

LevelEditor::LevelEditor()
{
	mainWindow.create(sf::VideoMode(1200,720), "Level-Editor");
	mainWindow.resetGLStates();

	inFocus = true;


}


void LevelEditor::initialize()
{
	
	interface->initialize();

	gameClock->initialize();

	initGui();

	numberOfGameObjects = 0;

	numberOfBgObjects = 0;

	viewPort.create(viewCanvas->GetAllocation().width, viewCanvas->GetAllocation().height);
	//view = viewPort.getView();

	camera.initialize(&viewPort);
	cameraSelection.setCamera(&camera);
	cameraSelection.setLevel(&level);
	cameraSelection.set(&selector, &hierarchyCamera);

	viewPort.setView(viewPort.getDefaultView());

	selector.properties = properties;
	selector.toolbar = toolBar;
	selector.desktop = &desktop;

	level.setSize(sf::IntRect(-20,20,40,40));


	level.setSelection(&selector, &hierarchyLevel);


	interface->initEditor(this, &level, &levelObjects, &desktop);
	interface->initView(&mainWindow, &viewCanvas, &viewPort, &camera);
	interface->initSelector(&selector);

	//moving = false;

	gameIsRunning = false;

	sf::View view = camera.getView();
	view.setSize((sf::Vector2f)viewPort.getSize());
	camera.setView(view);



	//LevelObject* rock = new LevelObject;

	//BombComponent* ai = new BombComponent();
	//rock->setAiComponent(ai);
	//addObject(rock);


}

void LevelEditor::initGui()
{
	guiWindow = sfg::Window::Create(sfg::Window::BACKGROUND);


	sfg::Box::Ptr mainBox = sfg::Box::Create(sfg::Box::VERTICAL, 2.0f);
	{
		sfg::Box::Ptr topBar = sfg::Box::Create(sfg::Box::HORIZONTAL);
		{
			sfg::Button::Ptr file = sfg::Button::Create("file");
			topBar->Pack(file, false);
			sfg::Button::Ptr add = sfg::Button::Create("add");
			topBar->Pack(add, false);


		}
		//mainBox->Pack(topBar, false);

		toolBar = sfg::Box::Create(sfg::Box::HORIZONTAL);
		{
			sfg::Button::Ptr _new = sfg::Button::Create("New");
			_new->GetSignal(sfg::Widget::OnLeftClick).Connect(&LevelEditor::shutdown, this);
			toolBar->Pack(_new, false);

			sfg::Button::Ptr save = sfg::Button::Create("Save");
			save->GetSignal(sfg::Widget::OnLeftClick).Connect(&Interface::saveLevel, interface);
			toolBar->Pack(save, false);

			sfg::Button::Ptr load = sfg::Button::Create("Load");
			load->GetSignal(sfg::Widget::OnLeftClick).Connect(&Interface::loadLevel, interface);
			toolBar->Pack(load, false);

			toolBar->Pack(sfg::Separator::Create(sfg::Separator::VERTICAL), false);


			sfg::Button::Ptr _export = sfg::Button::Create("export");
			_export->GetSignal(sfg::Widget::OnLeftClick).Connect(&Interface::exportLevel, interface);
			toolBar->Pack(_export, false);

			toolBar->Pack(sfg::Separator::Create(sfg::Separator::VERTICAL), false);

			sfg::Button::Ptr start = sfg::Button::Create("Start");
			start->GetSignal(sfg::Widget::OnLeftClick).Connect(&LevelEditor::startGame, this);
			toolBar->Pack(start, false);

			sfg::Button::Ptr stop = sfg::Button::Create("Stop");
			stop->GetSignal(sfg::Widget::OnLeftClick).Connect(&LevelEditor::stopGame, this);
			toolBar->Pack(stop, false);

			toolBar->Pack(sfg::Separator::Create(sfg::Separator::VERTICAL), false);




		}
		mainBox->Pack(toolBar, false);

		sfg::Box::Ptr workspace = sfg::Box::Create();
		{
			sfg::ScrolledWindow::Ptr hierarchyWindow = sfg::ScrolledWindow::Create();
			hierarchyWindow->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);
			{
				hierarchy = sfg::Box::Create(sfg::Box::VERTICAL);
				hierarchy->SetSpacing(5.0f);
				{
					sfg::Label::Ptr title = sfg::Label::Create("Hierarchy");
					hierarchy->Pack(title);

					hierarchy->Pack(sfg::Separator::Create());


					hierarchyLevel = sfg::Box::Create(sfg::Box::VERTICAL);
					{
						//hierarchyLevel->Pack(sfg::Label::Create("\n"));
						//hierarchyLevel->Pack(sfg::Separator::Create());
					}
					hierarchy->Pack(hierarchyLevel);

					hierarchy->Pack(sfg::Separator::Create());

					hierarchyCamera = sfg::Box::Create(sfg::Box::VERTICAL);
					{
						//hierarchyCamera->Pack(sfg::Label::Create("\n"));
						//hierarchyCamera->Pack(sfg::Separator::Create());

					}
					hierarchy->Pack(hierarchyCamera);

					hierarchy->Pack(sfg::Separator::Create());




					hierarchyBackgroundObjects = sfg::Box::Create(sfg::Box::VERTICAL);
					{
						sfg::Box::Ptr b = sfg::Box::Create(sfg::Box::HORIZONTAL);
						{
							b->Pack(sfg::Label::Create("Background Objects"));
							sfg::Button::Ptr add = sfg::Button::Create("Add");
							add->GetSignal(sfg::Button::OnLeftClick).Connect(&Interface::addBgObject,interface);
							b->Pack(add);
						}
						hierarchy->Pack(b);
					}
					hierarchy->Pack(hierarchyBackgroundObjects);

					hierarchy->Pack(sfg::Separator::Create());

					hierarchyGameObjects = sfg::Box::Create(sfg::Box::VERTICAL);
					{
						hierarchy->Pack(sfg::Label::Create("GameObjects"));
						sfg::Box::Ptr b = sfg::Box::Create(sfg::Box::HORIZONTAL);
						{
							sfg::Button::Ptr newObject = sfg::Button::Create("New");
							newObject->GetSignal(sfg::Button::OnLeftClick).Connect(&Interface::newObject, interface);
							b->Pack(newObject);

							sfg::Button::Ptr importObject = sfg::Button::Create("Import");
							importObject->GetSignal(sfg::Button::OnLeftClick).Connect(&Interface::importObject, interface);
							b->Pack(importObject);
						}
						hierarchy->Pack(b);

						hierarchy->Pack(sfg::Separator::Create());
					}
					hierarchy->Pack(hierarchyGameObjects);

				}
				hierarchyWindow->AddWithViewport(hierarchy);
				hierarchyWindow->SetRequisition(sf::Vector2f(180.0f,50.0f));
			}
			workspace->Pack(hierarchyWindow, false);


			viewCanvas = sfg::Canvas::Create();
			viewCanvas->SetRequisition(sf::Vector2f(400.0f,300.0f));
					
			

			viewCanvas->GetSignal(sfg::Widget::OnMouseMove).Connect			(&Interface::onMouseMove,interface);
			viewCanvas->GetSignal(sfg::Widget::OnMouseLeftPress).Connect	(&Interface::onLeftPress, interface);
			viewCanvas->GetSignal(sfg::Widget::OnMouseLeftRelease).Connect	(&Interface::onLeftRelease, interface);
			viewCanvas->GetSignal(sfg::Widget::OnRightClick).Connect		(&Interface::onRightClick, interface);

			workspace->Pack(viewCanvas);

			sfg::ScrolledWindow::Ptr propertyWindow = sfg::ScrolledWindow::Create();
			propertyWindow->SetScrollbarPolicy(sfg::ScrolledWindow::HORIZONTAL_AUTOMATIC | sfg::ScrolledWindow::VERTICAL_AUTOMATIC);
			{
				sfg::Box::Ptr propertyBox = sfg::Box::Create(sfg::Box::VERTICAL);
				//propertyBox->SetRequisition(sf::Vector2f(150.0f,50.0f));
				{
					sfg::Label::Ptr title = sfg::Label::Create("Properties");
					propertyBox->Pack(title);
					properties = sfg::Notebook::Create();
					properties->SetScrollable(true);
					properties->SetRequisition(sf::Vector2f(225.0f, 50.0f));
					{
						properties->AppendPage(sfg::Label::Create(""), sfg::Label::Create(""));
					}
					propertyBox->Pack(properties);
					//sfg::Window::Ptr window = sfg::Window::Create(sfg::Window::BACKGROUND);
					//propertieBox->Pack(window);
				}

				propertyWindow->AddWithViewport(propertyBox);
				propertyWindow->SetRequisition(sf::Vector2f(280.0f,50.0f));
			}
			workspace->Pack(propertyWindow, false);
		}
		mainBox->Pack(workspace);


	}
	guiWindow->Add(mainBox);



	guiWindow->SetAllocation(sf::FloatRect(sf::Vector2f(0.0f,0.0f),(sf::Vector2f)mainWindow.getSize()));
	desktop.Add(guiWindow);





}
void LevelEditor::run()
{

	while (mainWindow.isOpen())
	{

		clock.restart();

		if (gameIsRunning)
		{
			if (!prototype.frame())
			{


				//prototype.shutdownLevel();
				
				prototype.shutdown();

				prototype.initialize();

				

				restartGame();
				
				prototype.setGameOver(false);

				prototype.setDebug(true);

			}
		}

		sf::Event event;
		while (mainWindow.pollEvent(event))
		{
			if (event.type == sf::Event::GainedFocus)
				inFocus = true;


			if (inFocus)
			{

				sf::IntRect canvasRect;

				canvasRect.left = viewCanvas->GetAbsolutePosition().x;
				canvasRect.top  = viewCanvas->GetAbsolutePosition().y;

				canvasRect.width = viewCanvas->GetAllocation().width;
				canvasRect.height = viewCanvas->GetAllocation().height;

				switch (event.type)
				{

				case sf::Event::Closed:
					mainWindow.close();
					break;
				case sf::Event::Resized:
					{
						//sf::View view = viewPort.getView();

						sf::View view = camera.getView();

						guiWindow->SetAllocation(sf::FloatRect(sf::Vector2f(0.0f,0.0f),(sf::Vector2f)mainWindow.getSize()));

						viewPort.create(viewCanvas->GetAllocation().width, viewCanvas->GetAllocation().height);

						sf::Vector2f oldSize = view.getSize();

						view.setSize((sf::Vector2f)viewPort.getSize());
						view.setCenter(view.getCenter().x * view.getSize().x / oldSize.x, view.getCenter().y * view.getSize().y / oldSize.y);

						//viewPort.setView(view);

						camera.updateRenderTarget();

						camera.setView(view);

					}
					break;
				case sf::Event::MouseWheelMoved:
					{
						float factor;
						float delta = event.mouseWheel.delta * 1.1f;

						if (delta == 0)
							break;

						if (delta > 0)
						{
							factor = delta;
						}
						else if(delta < 0)
						{
							factor = -1.0f/delta;
						}

						//sf::View view = viewPort.getView();

						sf::View view = camera.getView();

						view.zoom( factor);

						camera.setView(view);
						//viewPort.setView(view);
					}
					break;

				case sf::Event::LostFocus:
					inFocus = false;
					break;

				case sf::Event::KeyPressed:
					if (viewCanvas->HasFocus())
						interface->onKeyPress();
					break;


				}
			}

			desktop.HandleEvent(event);
		}




		desktop.Update(clock.getElapsedTime().asSeconds());

		selector.update();

		//viewPort.setView(view);

		viewPort.clear();

		camera.draw(levelObjects.begin, &backGroundObjects, level, true);
		selector.draw(viewPort, &camera);


		viewPort.display();


		sf::Sprite renderSprite(viewPort.getTexture());


		mainWindow.setView(mainWindow.getDefaultView());

		mainWindow.draw(renderSprite);

		mainWindow.clear();

		viewCanvas->Bind();
		viewCanvas->Clear(sf::Color(255,255,255,255));
		viewCanvas->Draw(renderSprite);
		viewCanvas->Display();
		viewCanvas->Unbind();
		mainWindow.setActive(true);


		gui.Display(mainWindow);


		mainWindow.display();



	}
}


void LevelEditor::shutdown()
{
	selector.clearSelection();

	
	hierarchyGameObjects->RemoveAll();
	hierarchyBackgroundObjects->RemoveAll();

	hierarchy->RefreshAll();

	for (int i = 0; i < bgSelections.size(); i++)
	{
		delete bgSelections[i];
	}
	bgSelections.clear();


	levelObjects.shutdown();
	backGroundObjects.shutdown();



	level.shutdown();
}

void LevelEditor::save(std::string filename)
{
	std::fstream levelStream("Levels/" + filename, std::ios::out | std::ios::trunc | std::ios::binary);

	int versionNumber = VERSION;

	levelStream.write((char*)&versionNumber, sizeof(versionNumber));

	level.write(levelStream);

	camera.write(levelStream);
	
	levelStream.write((char*)&numberOfGameObjects, sizeof(numberOfGameObjects));
	GOIterator* it;
	for(it = levelObjects.begin; it; it = it->next)
	{
		it->object->write(levelStream);
	}

	levelStream.write((char*)&numberOfBgObjects, sizeof(numberOfBgObjects));
	BgOIterator* it2;
	for(it2 = backGroundObjects.begin; it2; it2 = it2->next)
	{
		it2->object->write(levelStream);
	}


	levelStream.close();

}

void LevelEditor::load(std::string filename, bool objects)
{

	std::fstream levelStream("Levels/" + filename, std::ios::in | std::ios::binary);

	int versionNumber;

	levelStream.read((char*)&versionNumber, sizeof(versionNumber));



	level.read(levelStream, 0, versionNumber);
	level.selection.updateProperty();

	if (versionNumber >= 2)
		camera.read(levelStream, versionNumber);


	unsigned int n = 0;
	levelStream.read((char*)&n, sizeof(unsigned int));

	if (objects)
	{
		for (int i = 0; i < n; i++)
		{
			LevelObject* levelObject = new LevelObject();
			levelObject->read(levelStream, 0, versionNumber);
			addObject(levelObject);
		}
	}


	if (versionNumber >= 2)
	{
		unsigned int nBg;
		levelStream.read((char*)&nBg, sizeof(nBg));

		for (int i = 0; i < nBg; i++)
		{
			BackGroundObject* bgObject = new BackGroundObject();
			bgObject->read(levelStream, versionNumber);
			addBackGroundObject(bgObject);
		}
	}

	levelStream.close();

}


void LevelEditor::exportPng(std::string filename)
{
	sf::RenderTexture renderTexture;

	renderTexture.create(level.getSize().width * mToPx, level.getSize().height * mToPx);



	sf::View _view(sf::FloatRect(
		level.getSize().left * mToPx,
		level.getSize().top * -mToPx,
		level.getSize().width * mToPx,
		level.getSize().height * mToPx
		));

	//renderTexture.setView(_view);

	sf::View oldView = camera.getView();

	camera.setView(_view);
	camera.draw(&renderTexture, levelObjects.begin, &backGroundObjects, level, true);

	camera.setView(oldView);

	renderTexture.display();



	sf::Image image = renderTexture.getTexture().copyToImage();
	image.flipVertically();
	image.flipVertically();

	image.saveToFile("Export/" +  filename);


}



void LevelEditor::startGame()
{
	if (!gameIsRunning) 
	{
		prototype.initialize();

		//prototype.setLevel(level.getLevel(&prototype));

		//GOIterator* it = levelObjects.begin;
		//for(; it; it = it->next)
		//{
		//	LevelObject* object = dynamic_cast<LevelObject*>(it->object);

		//	prototype.addObject(object->getGameObject(&prototype));
		//}

		//prototype.initializeGameObjects();


		//BgOIterator* bgIt = backGroundObjects.begin;
		//for(; bgIt; bgIt = bgIt->next)
		//{
		//	BackGroundObject* object = new BackGroundObject(*bgIt->object);

		//	prototype.addBackGroundObject(object);
		//}

		restartGame();

		gameIsRunning = true;
	}

}


void LevelEditor::restartGame()
{
	prototype.setLevel(level.getLevel(&prototype));


	prototype.setCamera(camera);

	GOIterator* it = levelObjects.begin;
	for(; it; it = it->next)
	{
		LevelObject* object = dynamic_cast<LevelObject*>(it->object);

		prototype.addObject(object->getGameObject(&prototype));
	}
	prototype.initializeGameObjects();

	BgOIterator* bgIt = backGroundObjects.begin;
	for(; bgIt; bgIt = bgIt->next)
	{
		BackGroundObject* object = new BackGroundObject(*bgIt->object);

		prototype.addBackGroundObject(object);
	}

	gameClock->startNewFrame();


}

void LevelEditor::stopGame()
{
	if (gameIsRunning)
	{


		prototype.shutdown();

		gameIsRunning = false;


	}

}

void LevelEditor::addObject(LevelObject* object)
{




	levelObjects.addObject(object);
	object->setSelection(this, &selector, &hierarchyGameObjects);
	object->initialize();

	numberOfGameObjects++;


	bool nameIsTaken = false;
	do
	{
		if (nameIsTaken)
		{
			std::string name = object->getName();

			if (isdigit(name.back()))
			{
				int i = atoi(&name.back());
				i++;

				std::stringstream ss;

				ss << i;

				name.replace(name.size()-1, ss.str().size(), ss.str());
			}
			else
			{
				name += "1";
			}

			object->setName(name);
			nameIsTaken = false;
		}

		for (GOIterator* it = levelObjects.begin; it; it = it->next)
		{
			if (it->object->getName() == object->getName() && it->object != object)
				nameIsTaken = true;
		}

	}while(nameIsTaken);


}

void LevelEditor::deleteObject(LevelObject* object)
{

	hierarchyGameObjects->Remove(object->selection.hierarchyButton);

	hierarchy->RefreshAll();

	levelObjects.deleteObject(object);

	levelObjects.clean();

	numberOfGameObjects--;

}




void LevelEditor::addBackGroundObject(BackGroundObject* object)
{
	backGroundObjects.addObject(object);
	bgSelections.push_back(new SelectionBgObject(this, object, &selector, &hierarchyBackgroundObjects));

	numberOfBgObjects++;
}

void LevelEditor::resortBackGroundObject(BackGroundObject* object)
{
	backGroundObjects.resortObject(object);
}


void LevelEditor::deleteBackGroundObject(BackGroundObject* object)
{


	std::vector<SelectionBgObject*>::iterator it;
	for (it = bgSelections.begin(); it != bgSelections.end(); it++)
	{
		if ((*it)->getObject() == object)
		{

			hierarchyBackgroundObjects->Remove((*it)->hierarchyButton);
			hierarchy->RefreshAll();


			delete (*it);

			bgSelections.erase(it);

			numberOfBgObjects--;

			break;
			
		}
	}

	backGroundObjects.deleteObject(object);
}