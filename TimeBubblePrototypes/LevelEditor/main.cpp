#include "LevelEditor.h"
#include <SFGUI\SFGUI.hpp>

#include <SFML\Graphics.hpp>

//#include <vld.h>



int main()
{


	LevelEditor levelEditor;
	levelEditor.initialize();

	levelEditor.run();

	levelEditor.shutdown();

	return 0;
}
	
