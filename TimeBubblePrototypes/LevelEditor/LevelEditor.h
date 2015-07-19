#ifndef LEVEL_EDITOR_H
#define LEVEL_EDITOR_H

#include <Prototype.h>
#include <SFML\Graphics.hpp>
#include <SFGUI\SFGUI.hpp>
#include "LevelObject.h"
#include <ObjectContainer.h>
#include "Selector.h"
#include "SelectionObject.h"
#include "SelectionCamera.h"
#include <BgObjectContainer.h>
#include <BackGroundObject.h>
#include "EditorLevel.h"
#include "SelectionBgObject.h"

#define VERSION 16

class LevelEditor
{
public:

	LevelEditor();

	void initialize();
	void run();
	void shutdown();

	void startGame();
	void restartGame();
	void stopGame();

	void save(std::string filename);
	void load(std::string filename, bool objects = true);
	void exportPng(std::string filename);

	void addObject(LevelObject* object);
	void deleteObject(LevelObject* object);

	void addBackGroundObject(BackGroundObject* object);
	void deleteBackGroundObject(BackGroundObject* object);
	void resortBackGroundObject(BackGroundObject* object);


private:
	sfg::SFGUI gui;

	bool inFocus;

	sf::Clock clock;


	Camera camera;
	SelectionCamera cameraSelection;

	Prototype prototype;
	bool gameIsRunning;

	EditorLevel level;

	sf::RenderWindow mainWindow;

	sfg::Desktop desktop;

	sfg::Window::Ptr guiWindow;

	sfg::Canvas::Ptr viewCanvas;
	sf::RenderTexture viewPort;
	

	ObjectContainer levelObjects;
	unsigned int numberOfGameObjects;

	unsigned int numberOfBgObjects;
	BgObjectContainer backGroundObjects;
	std::vector<SelectionBgObject*> bgSelections;
	

	sfg::Box::Ptr toolBar;

	sfg::Box::Ptr hierarchy;
	sfg::Box::Ptr hierarchyLevel;
	sfg::Box::Ptr hierarchyCamera;
	sfg::Box::Ptr hierarchyBackgroundObjects;
	sfg::Box::Ptr hierarchyGameObjects;
	//std::vector<SelectionObject> hierarchyObjects;

	sfg::Notebook::Ptr properties;

	Selector selector;

	void initGui();


};

#endif