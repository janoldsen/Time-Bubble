#ifndef INTERFACE_H
#define INTERFACE_H

#include <SFML\Graphics.hpp>
#include <SFGUI\SFGUI.hpp>
#include "Selector.h"
#include "LevelEditor.h"
#include "EditorLevel.h"
#include "ObjectContainer.h"

class Interface
{
public:

	static Interface* getInstance();

	void initialize();

	void initEditor(LevelEditor* _editor, EditorLevel* _level, ObjectContainer* _gameObjects, sfg::Desktop* _desktop);
	void initView(sf::RenderWindow* _mainWindow, sfg::Canvas::Ptr* _viewCanvas, sf::RenderTarget* _viewPort, Camera* _camera);

	void initSelector(Selector* _selector);


	void onLeftPress();
	void onRightClick();
	void onLeftRelease();
	void onMouseMove();
	void onKeyPress();

	void addVertex(int _segmentID);
	void addSegment();

	void newObject();
	void importObject();

	void addBgObject();

	void exportLevel();
	void saveLevel();
	void loadLevel();


private:

	static Interface instance;

	Interface();
	Interface(Interface&);
	Interface& operator=(Interface&);

	enum
	{
		IDLE,
		SELECTING,
		SELECTIONTRANSLATION,
		ADDINGVERTEX,
		SETTINGVERTEX,
		IMPORTINGOBJECT,
		WINDOWISOPEN,
		ADDINGBGOBJECT,

	} state;
	bool movingView;

	LevelEditor* editor;
	EditorLevel* level;
	ObjectContainer* gameObjects;
	sfg::Desktop* desktop;


	sf::Vector2f position;
	sf::Vector2f startPosition;

	sf::RenderWindow* mainWindow;
	sfg::Canvas::Ptr* viewCanvas;
	sf::RenderTarget* viewPort;
	Camera* camera;
	//sf::View*	view;

	Selector* selector;

	int segmentId;
	int count;
	b2Vec2 vertecies[2];

	sfg::Window::Ptr window;
	sfg::Entry::Ptr entry;

	LevelObject* tempObject;
	void doImportObject();


	void doExportLevel();
	void doSaveLevel();
	void doLoadLevel();

};

#define interface Interface::getInstance() 

#endif