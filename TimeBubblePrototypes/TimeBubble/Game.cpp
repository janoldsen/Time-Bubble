#include "Game.h"
#include "Button.h"
#include "GameClock.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <Windows.h>

void Game::initialize()
{

	//window.create(sf::VideoMode(800,600),"Prototype_1", sf::Style::Close);


	//for (int i = 0; i < 3; i++)
	//{
	//	for (int j = 0; j < 27; j++)
	//		savegame[j] = 0;
	//	saveSaveGame(i);

	//}


	imageManager->load("res/start.txt");


	gameClock->initialize();


	worldsPage.lastPage = &mainPage;
	for (int i = 0;i<3;i++)
		worldPage[i].lastPage = &worldsPage;
	loadPage.lastPage = &mainPage;
	optionPage.lastPage = &mainPage;

	//window.create(sf::VideoMode::getFullscreenModes()[0], "Time Bubble", sf::Style::Fullscreen);
	window.create(sf::VideoMode(800,600),"");
	window.setJoystickThreshold(5.0f);


	//mainPage

	PageButton* play = new PageButton();
	play->set("res/buttons/play_button.scml", sf::FloatRect(-25.0f, -25.0f, 50.0f, 50.0f));
	play->setPosition(sf::Vector2f(0.25f,0.75f));
	play->game = this;
	play->page = &worldsPage;
	mainPage.addButton(play);
		
	//PageButton* load = new PageButton();
	//load->set("res/buttons/load_button.scml", sf::FloatRect(-25.0f, -25.0f, 50.0f, 50.0f));
	//load->setPosition(sf::Vector2f(0.41f,0.75f));
	//load->game = this;
	//load->page = &loadPage;
	//mainPage.addButton(load);
	
	PageButton* option = new PageButton();
	option->set("res/buttons/option_button.scml", sf::FloatRect(-25.0f, -25.0f, 50.0f, 50.0f));
	option->setPosition(sf::Vector2f(0.5f,0.75f));
	option->game = this;
	option->page = &optionPage;
	mainPage.addButton(option);

	QuitButton* quit = new QuitButton();
	quit->set("res/buttons/quit_button.scml", sf::FloatRect(-25.0f, -25.0f, 50.0f, 50.0f));
	quit->setPosition(sf::Vector2f(0.75f,0.75f));
	quit->game = this;
	mainPage.addButton(quit);

	play->next = option;
	//load->previous = play;
	//load->next = option;
	option->previous = play;
	option->next = quit;
	quit->previous = option;


	//worldsPage
	
	worldButtons[0] = new WorldButton;
	worldButtons[0]->set("res/buttons/world0.scml", sf::FloatRect(-75.0f, -50.5f, 150.0f, 100.0f));
	worldButtons[0]->setPosition(sf::Vector2f(0.5,0.25));
	worldButtons[0]->game = this;
	worldButtons[0]->page = &worldPage[0];
	worldButtons[0]->numRequiredStar = 0;
	worldButtons[0]->currNumStar = &numStars;
	worldsPage.addButton(worldButtons[0]);
	
	worldButtons[1] = new WorldButton;
	worldButtons[1]->set("res/buttons/world1.scml", sf::FloatRect(-75.0f, -50.5f, 150.0f, 100.0f));
	worldButtons[1]->setPosition(sf::Vector2f(0.75,0.5));
	worldButtons[1]->game = this;
	worldButtons[1]->page = &worldPage[1];
	worldButtons[1]->numRequiredStar = 7;
	worldButtons[1]->currNumStar = &numStars;
	worldButtons[1]->locked = true;
	worldsPage.addButton(worldButtons[1]);

	worldButtons[2] = new WorldButton;
	worldButtons[2]->set("res/buttons/world2.scml", sf::FloatRect(-75.0f, -50.5f, 150.0f, 100.0f));
	worldButtons[2]->setPosition(sf::Vector2f(0.25,0.5));
	worldButtons[2]->game = this;
	worldButtons[2]->page = &worldPage[2];
	worldButtons[2]->numRequiredStar = 14;
	worldButtons[2]->currNumStar = &numStars;
	worldButtons[2]->locked = true;
	worldsPage.addButton(worldButtons[2]);
	
	worldButtons[0]->previous = worldButtons[2];
	worldButtons[0]->next = worldButtons[1];
	worldButtons[1]->previous = worldButtons[0];
	worldButtons[1]->next = worldButtons[2];
	worldButtons[2]->previous = worldButtons[1];
	worldButtons[2]->next = worldButtons[0];


	//worldPage

	std::ifstream in;
	std::string level;
	
	for (int i = 0; i < 27; i++)
		levelButtons[i] = 0;

	for (int i = 0; i < 3; i++)
	{

		std::stringstream ss;
		ss << "levels/world" << i << ".txt";

		//std::cout << ss.str() << std::endl;
		in.open(ss.str());
		int u = 0, v = 0;
		for (int j = 0; std::getline(in, level); j++)
		{
			std::cout << j + i * 9 << "	" << level << std::endl;
			


			levels[i][j] = level;

			levelButtons[j + i * 9] = new LevelButton();
			levelButtons[j + i * 9]->game = this;

			levelButtons[j + i * 9]->worldId = i;
			levelButtons[j + i * 9]->levelId = j;
			//levelButton->set("res/buttons/" + level + ".scml", sf::FloatRect(-25.0f, -25.0f, 50.0f, 50.0f));
			levelButtons[j + i * 9]->set("res/buttons/play/play.scml", sf::FloatRect(-25.0f, -25.0f, 50.0f, 50.0f));
			levelButtons[j + i * 9]->setPosition(sf::Vector2f(1.0/4.0 * (1.0 + u), 1.0/4.0 * (1.0 + v)));
			levelButtons[j + i * 9]->star = false;

			if (j > 0)
			{
				levelButtons[(j-1) + i * 9]->next = levelButtons[j + i * 9];
				levelButtons[j + i * 9]->previous = levelButtons[(j-1) + i * 9];

			}


			worldPage[i].addButton(levelButtons[j + i * 9]);

			if (++u >= 3)
			{
				u = 0;
				v++;
			}
		}
		in.close();
	}


	//pausePage

	ResetButton* resetButton = new ResetButton();
	resetButton->set("res/buttons/reset_button.scml", sf::FloatRect(-100, -25, 200, 50));
	resetButton->setPosition(sf::Vector2f(0.5,0.3));
	resetButton->game = this;
	pausePage.addButton(resetButton);

	PageButton* optionButton = new PageButton();
	optionButton->set("res/buttons/option_button.scml", sf::FloatRect(-100, -25, 200, 50));
	optionButton->setPosition(sf::Vector2f(0.5,0.4));
	optionButton->game = this;
	optionButton->page = &optionPage;
	pausePage.addButton(optionButton);

	StopLevelButton* stopButton = new StopLevelButton();
	stopButton->set("res/buttons/back_button.scml", sf::FloatRect(-100, -25, 200, 50));
	stopButton->setPosition(sf::Vector2f(0.5,0.5));
	stopButton->game = this;
	pausePage.addButton(stopButton);

	
	resetButton->next = optionButton;
	optionButton->previous = resetButton;
	optionButton->next = stopButton;
	stopButton->previous = optionButton;


	//optionPage

	setWindowButton* fullscreen = new setWindowButton;
	fullscreen->set("res/buttons/full_screen.scml", sf::FloatRect(-25.0f, -25.0f, 50.0f, 50.0f));
	fullscreen->setPosition(sf::Vector2f(0.5,0.4));
	fullscreen->game = this;
	fullscreen->videoMode = sf::VideoMode::getFullscreenModes()[0];
	fullscreen->fullscreen = true;
	optionPage.addButton(fullscreen);

	setWindowButton* big = new setWindowButton;
	big->set("res/buttons/middle_screen.scml", sf::FloatRect(-25.0f, -25.0f, 50.0f, 50.0f));
	big->setPosition(sf::Vector2f(0.5,0.5));
	big->game = this;
	big->videoMode = sf::VideoMode(1280,720);
	big->fullscreen = false;
	optionPage.addButton(big);

	setWindowButton* _small = new setWindowButton;
	_small->set("res/buttons/small_screen.scml", sf::FloatRect(-25.0f, -25.0f, 50.0f, 50.0f));
	_small->setPosition(sf::Vector2f(0.5,0.6));
	_small->game = this;
	_small->videoMode = sf::VideoMode(800,600);
	_small->fullscreen = false;
	optionPage.addButton(_small);

	VolumeButton* volume = new VolumeButton;
	volume->setPosition(sf::Vector2f(0.5,0.7));
	volume->game = this;
	volume->set(sf::FloatRect(-100.0,-25.0,200,50));
	volume->sprite = sf::Sprite(imageManager->getTexture("res/buttons/volume.png"));
	optionPage.addButton(volume);


	fullscreen->next = big;
	big->previous = fullscreen;
	big->next = _small;
	_small->previous = big;




	//loadPage

	for (int i = 0; i < 3; i++)
	{
		savegameButtons[i] = new SavegameButton();
		savegameButtons[i]->game = this;
		savegameButtons[i]->id = i;
		savegameButtons[i]->set("res/buttons/savegame.scml", sf::FloatRect(-225.0f, -50.0f, 450.0f, 100.0f));
		savegameButtons[i]->setPosition(sf::Vector2f(0.5,0.25 + (i * 0.2f)));
		loadPage.addButton(savegameButtons[i]);
	}

	SCML::Data* data = imageManager->getData("res/background/start_screen.scml");
	fs.load(data);
	
	backGround.setFileSystem(&fs);
	backGround.setScreen(&window);
	backGround.entity = 0;
	backGround.animation = 0;
	backGround.key = 0;
	backGround.time = 0;
	backGround.load(data);

	
	currPage = &mainPage;

	gameIsRunning = false;


	title_music.openFromFile("music/title_screen.wav");
	title_music.setLoop(true);
	title_music.play();

	
	loadSaveGame(0);

	currWorld = -1;

}


void Game::run()
{
	while (window.isOpen())
	{

		if (gameIsRunning)
		{
			frame();

			if (pause)
			{
				currPage->update(window);
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
					currPage->mouseClick();
			}

		}
		else 
		{
			menuFrame();
		}

	}
}

void Game::menuFrame()
{

	gameClock->startNewFrame();

	title_music.setVolume(volume);
	
	
	pollMenuEvent();


	currPage->update(window, controllerActive());

	window.clear();

	backGround.update(gameClock->getElapsedTime() * 1000.0f);
	backGround.draw(0.5f * window.getSize().x, 0.5f * window.getSize().y, 0.0f, window.getSize().y /1080.0f, window.getSize().y /1080.0f);
	
	currPage->draw(&window);

	window.display();

	Sleep(20.0f);
}

void Game::setPage(Page* page)
{
	currPage = page;
	currPage->resetButtons();
}



bool Game::startLevel(int worldId, int levelId)
{
	window.setJoystickThreshold(0.1f);

	title_music.pause();
	
	if (worldId != currWorld)
	{
		std::cout << std::endl;

		if (currWorld >= 0 && currWorld < 3)
		{
			std::stringstream unLoad;
			unLoad << "res/World" << currWorld << ".txt";
			imageManager->unLoad(unLoad.str());
		}

		std::stringstream load;
		load << "res/World" << worldId << ".txt";
		imageManager->load(load.str());

	}


	gameIsRunning = true;

	Prototype::initialize();
	
	if (!loadLevel("levels/" + levels[worldId][levelId] + ".lvl"))
	{
		gameIsRunning = false;
		shutdownLevel();
		return false;
	}


	currWorld = worldId;
	currLevel = levelId;
	
	//currPage = &pausePage;
	setPage(&pausePage);
}



void Game::stopLevel(int i)
{

	window.setJoystickThreshold(5.0f);

	shutdownLevel();
	title_music.play();
	
	//currPage = &worldPage[currWorld];
	setPage(&worldPage[currWorld]);
	
	if (i > 0)
	{
		if (savegame[currWorld * 9 + currLevel] != 2 && i == 2)
		{
			savegame[currWorld * 9 + currLevel] = 2;
			numStars++;
			levelButtons[currWorld * 9 + currLevel]->star = true;
		}
		else if (savegame[currWorld * 9 + currLevel] < 1)
		{
			savegame[currWorld * 9 + currLevel] = 1;
		}

		saveSaveGame(0);
		levelButtons[currWorld * 9  + currLevel + 1]->locked = false;
	}

	if (i == 0)
	{
		gameIsRunning = false;
	}
	else if (i == 1 || i == 2)
	{
		if (currLevel < 8)
		{
			startLevel(currWorld, currLevel+1);
		}
		else
		{
			//currPage = &worldPage[currWorld+1];
			gameIsRunning = false;
			setPage(&worldPage[currWorld]);
		}
	}	
	
}


void Game::addDraw()
{
	if (pause)
	{

		sf::View oldView = window.getView();
		window.setView(window.getDefaultView());

		
		currPage->draw(&window);

		window.setView(oldView);


	}

}




void Game::pollMenuEvent()
{
	sf::Event event;

	while(window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			window.close();

		if (event.type == sf::Event::KeyPressed)
		{

			if (event.key.code == sf::Keyboard::Escape)
			{

				if (currPage->lastPage)
				{

					//currPage = currPage->lastPage;
					setPage(currPage->lastPage);

				}
				else
				{
					window.close();
				}
			}
		}

		if (event.type == sf::Event::MouseButtonPressed)
		{
			controller = false;

			if (event.mouseButton.button == sf::Mouse::Left)
				currPage->mouseClick();
		}

		if (event.type == sf::Event::MouseMoved)
		{
			controller = false;
		}

		if (event.type == sf::Event::JoystickMoved)
		{
			controller = true;

			if (event.joystickMove.axis == sf::Joystick::X)
			{
				std::cout << event.joystickMove.position << std::endl;

				if (event.joystickMove.position > 90)
					currPage->controllerRight();
				else if(event.joystickMove.position < -90)
					currPage->controllerLeft();

			}

		}

		if (event.type == sf::Event::JoystickButtonPressed)
		{
			controller = true;

			if (event.joystickButton.button == 0)
				currPage->mouseClick();

			if (event.joystickButton.button == 1)
			{
				if (currPage->lastPage)
				{

					//currPage = currPage->lastPage;
					setPage(currPage->lastPage);

				}
			}
		}
	}
}


void Game::setWindow(sf::VideoMode videoMode, bool fullscreen)
{
	sf::View oldView = window.getView();



	window.create(videoMode, "Time-Bubble", fullscreen ? sf::Style::Fullscreen : sf::Style::Default);


	if (gameIsRunning)
	{
		//window.setView(oldView);

		//if (videoMode.width / videoMode.height != 4.0f/3.0f)
		//{
		//	//sf::View view = window.getView();
		//	oldView.setViewport(sf::FloatRect(0.125f, 0, 0.75, 1.0));
		//}

		window.setView(oldView);

	}


}


void Game::setPause(bool flag)
{
	pause = flag;
	if (flag)
		//currPage = &pausePage;
			setPage(&pausePage);
}




void Game::loadSaveGame(int id)
{
	std::stringstream ss;
	ss << "savegame" << id << ".txt";

	std::ifstream in;
	in.open(ss.str());

	numStars = 0;

	for (int i = 0; i < 26; i++)
	{
		std::cout << i << std::endl;
		in >> savegame[i];
		if (levelButtons[i + 1])
		{
			std::cout << i << "	" << savegame[i] << std::endl;
			if (savegame[i] > 0)
			{
				levelButtons[i + 1]->locked = false;
				worldButtons[i / 9]->locked = false;

				if (savegame[i] == 2)
				{
					numStars++;
					levelButtons[i]->star = true;
				}
			}
			else
				levelButtons[i + 1]->locked = true;
		}
	}

	levelButtons[0]->locked = false;

	in.close();
}


void Game::saveSaveGame(int id)
{
	std::stringstream ss;
	ss << "savegame" << id << ".txt";

	std::ofstream out;
	out.open(ss.str());

	for (int i = 0; i < 26; i++)
	{
		out << savegame[i] << std::endl;
	}

	out.close();

}
