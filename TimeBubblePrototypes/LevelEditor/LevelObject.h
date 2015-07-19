#ifndef LEVEL_OBJECT_H
#define LEVEL_OBJECT_H

#include <GameObject.h>
#include <Prototype.h>
#include <Box2D\Box2d.h>
#include <vector>
#include "SelectionObject.h"
//#include <myMath.h>
#include "SelectionVertex.h"

class Selection;
class LevelEditor;

class LevelObject : public GameObject
{
public:
	LevelObject();

	void initialize();
	void shutdown();

	void draw(Camera* camera, sf::RenderTarget* target, bool debug = false, sf::Color color = sf::Color::Black);


	RendererComponent* getRendererComponent();
	AiComponent* getAiComponent();

	void exportObject();
	void importObject(std::string filename);
	

	void setName(std::string _name);
	void setSelection(LevelEditor* editor, Selector* selector, sfg::Box::Ptr* hierarchy);
	
	void checkSelection(sf::FloatRect& selectionRect/*, std::vector<Selection*>& selection*/);


	GameObject* getGameObject(Prototype* prototype);

	b2BodyDef bodyDef;
	
	std::vector<b2FixtureDef> fixtureDefs;
	std::vector<b2Shape*> shapes;
	

	bool hasBody;

	SelectionObject selection;

protected:
	void writeBody(std::fstream& stream);
	void readBody(std::fstream& stream, Prototype* _game, int versionNumber);





};



#endif