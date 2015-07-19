#include "Game.h"
#include <iostream>

//#include <vld.h>

int main()
{
	Game timeBubble;

	timeBubble.initialize();

	//std::string level;

	//std::cin >> level;

	//timeBubble.loadLevel("Levels/" + level);

	timeBubble.run();

	timeBubble.shutdown();


	return 0;
}