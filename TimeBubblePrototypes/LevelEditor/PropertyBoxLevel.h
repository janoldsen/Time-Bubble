#ifndef PROPERTY_BOX_LEVEL_H
#define PROPERTY_BOX_LEVEL_H

#include "PropertyBox.h"
#include <vector>
#include "PropertyBoxSegment.h"

typedef sfg::Entry::Ptr  Entry;
typedef sfg::Box::Ptr Box;


class EditorLevel;

class PropertyBoxLevel : public PropertyBox
{
public:

	~PropertyBoxLevel();

	void shutdown();

	void setLevel(EditorLevel& _level);

	void update();

	void build();

	void addSegment(b2FixtureDef* _segment);
	void deleteSegment(int segmentId);

	std::vector<PropertyBoxSegment*> segments;

private:

	EditorLevel* level;

	Entry backGround;
	Entry gameGround;
	Entry sizeLeft;
	Entry sizeTop;
	Entry sizeWidth;
	Entry sizeHeight;
	
	sfg::Button::Ptr addSegmentButton;

	void setBackGround();
	void setGameGround();
	void setSize();
	void _addSegment();







};

#endif