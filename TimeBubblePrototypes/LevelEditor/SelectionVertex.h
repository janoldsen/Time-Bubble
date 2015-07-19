#ifndef SELECTION_VERTEX_H
#define SELECTION_VERTEX_H

#include "Selection.h"
//#include "EditorLevel.h"
#include <Box2D\Common\b2Math.h>
#include "PropertyBoxSegment.h"

class EditorLevel;

class SelectionVertex : public Selection
{
public:


	SelectionVertex(Selector& _selector, EditorLevel* _level, b2Vec2* _vertex, int _segmentId, int _id);


	void select(bool add);
	void updateProperty();

	void deleteSelection();
	
	void move(sf::Vector2f amount);
	sf::Vector2f getPosition();

	std::string getName();

	int getSegmentId();
	int getVertexId();

	void unselect();
	void setVertex(b2Vec2* _vertex, int _segmentId,  int _id);

	void setSegmentId(int _segmentId);

	sfg::Box::Ptr getPropertyBox();


private:
	
	EditorLevel* level;
	b2Vec2* vertex;

	int id;
	int segmentId;

	PropertyBoxVertex propertyBox;


};


#endif