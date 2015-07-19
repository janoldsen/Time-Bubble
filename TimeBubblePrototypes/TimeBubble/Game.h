#ifndef GAME_H
#define GAME_H

#include <Prototype.h>
#include "Page.h"
#include <SFML\Audio.hpp>

class LevelButton;
class SavegameButton;
class PageButton;
class QuitButton;
class WorldButton;

class Game : public Prototype
{
public:
	void initialize();

	void run();

	void setPage(Page* page);


	bool startLevel(int worldId, int levelId);
	void stopLevel(int i);

	void setWindow(sf::VideoMode videoMode, bool fullscreen);

	void setPause(bool flag);

	void loadSaveGame(int id);
	void saveSaveGame(int id);


private:
	Page* currPage;
	Page* currPausePage;

	Page mainPage;
	Page worldsPage;
	Page worldPage[3];
	Page optionPage;
	Page loadPage;

	Page pausePage;

	SCML_SFML::Entity backGround;
	SCML_SFML::FileSystem fs;

	sf::View oldView;

	sf::Music title_music;

	void pollMenuEvent();

	//std::vector< std::vector<std::string> > levels;

	std::string levels[3][9];
	int savegame[26];
	SavegameButton* savegameButtons[3];
	LevelButton* levelButtons[27];
	int currWorld;
	int currLevel;

	WorldButton* worldButtons[3];

	int numStars;

	void addDraw();

	bool gameIsRunning;

	void menuFrame();

};

#endif