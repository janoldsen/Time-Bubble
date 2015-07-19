#ifndef SELECTION_LEVEL_H
#define SELECTION_LEVEL_H

#include "Selection.h"
//#include "EditorLevel.h"
#include "PropertyBoxLevel.h"

class EditorLevel;

class SelectionLevel : public Selection
{
public:
	SelectionLevel();

	void shutdown();

	void move(sf::Vector2f amount);
	sf::Vector2f getPosition();

	void updateProperty();

	std::string getName();

	void select(bool add);

	void setLevel(EditorLevel& _level);

	void addSegment();
	void addVertex(int segmentId, sfg::Box::Ptr vertexPropertyBox);
	void insertVertex(int segmentId, int position, sfg::Box::Ptr vertexPropertyBox);

	void deleteSegment(int segmentId);
	void deleteVertex(int segmentId, sfg::Box::Ptr vertexPropertyBox);
	

	sfg::Box::Ptr getPropertyBox();
	//sfg::Box::Ptr getToolbarBox();
	
	



private:
	
	EditorLevel* level;
	PropertyBoxLevel propertyBox;
	
	//sfg::Box::Ptr toolbarBox;
	sfg::Button::Ptr insertButton;
	sfg::Button::Ptr breakButton;
	sfg::Button::Ptr weldButton;

	void doInsertVertex();
	void doBreakVertex();
	void doWeldVertecies();


};


#endif