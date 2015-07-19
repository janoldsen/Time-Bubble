#ifndef EDITOR_LEVEL_H
#define EDITOR_LEVEL_H

#include <Level.h>
#include <vector>
#include "SelectionVertex.h"
#include <SFGUI\SFGUI.hpp>
#include "SelectionLevel.h"



//class Selection;
//class SelectionLevel;
class Selector;

class EditorLevel : public Level
{
public:


	EditorLevel();

	void shutdown();

	void drawGameGround(Camera* camera, sf::RenderTarget* target, bool debug = false, sf::Color color = sf::Color::Black);

	void checkSelection(sf::FloatRect& selectionRect);

	

	int addSegment(b2FixtureDef& fixtureDef);
	void setVertices(Selector& selector, int segmentId, b2Vec2* vertices, int count);
	void addVertex(Selector& selector, int segmentId, b2Vec2 position);
	void insertVertex(Selector& selector, int vertexId1, int segmentId);
	void breakVertex(Selector& selector, int vertexId, int segmentId);
	void weldVertices(Selector& selector, int vertexId1, int segmentId1, int vertexId2, int segmentId2);

	void setLoop(int segmentId, bool loop);


	void deleteSegment(int segmentId);
	void deleteVertex(int segmentId, int vertexId);


	b2BodyDef bodyDef;

	std::vector<b2FixtureDef*> fixtureDefs;
	std::vector<b2ChainShape*> shapes;

	std::vector< std::vector<SelectionVertex*> > selections;
	SelectionLevel selection;

	Level getLevel(Prototype* game);

	void setSelection(Selector* _selector,sfg::Box::Ptr* hierarchy);



private:
	void writeBody(std::fstream& stream);
	void readBody(std::fstream& stream, Prototype* game);

	Selector* selector;

};

#endif