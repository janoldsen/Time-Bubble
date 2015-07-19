#ifndef PROPERTY_BOX_SEGMENT_H
#define PROPERTY_BOX_SEGMENT_H

#include "PropertyBox.h"

#include <Box2D\Box2D.h>


class EditorLevel;

class PropertyBoxVertex : public PropertyBox
{
public:

	PropertyBoxVertex();
	
	void buildBox();

	void build();

	void update();

	void setVertex(b2Vec2* _vertex);

	void set();


private:
	sfg::Entry::Ptr x;
	sfg::Entry::Ptr y;

	b2Vec2* vertex;

	


};



class PropertyBoxSegment : public PropertyBox
{
public:
	PropertyBoxSegment();

	void update();

	void build(int _id);
	void setSegment(b2FixtureDef* _segement, EditorLevel* _level);
	void setId(int _id);


private:

	b2FixtureDef* segment;
	EditorLevel* level;
	int id;

	sfg::Entry::Ptr friction;
	sfg::Entry::Ptr userData;
	sfg::Button::Ptr addVertecies;
	
	sfg::ComboBox::Ptr loop;
	
	void add();
	void setFriction();
	void setUserData();
	void setLoop();
};


#endif