#include "SelectionVertex.h"
#include <myMath.h>
#include "Selector.h"
#include "EditorLevel.h"

SelectionVertex::SelectionVertex(Selector& _selector, EditorLevel* _level, b2Vec2* _vertex, int _segmentId, int _id)

{
	level = _level;
	vertex = _vertex;
	//selector = &_selector;

	id = _id;
	segmentId = _segmentId;

	propertyBox.buildBox();
	propertyBox.setVertex(_vertex);

	Selection::set(&_selector, &propertyBox.get());

	propertyBox.build();

	_hasProperty = false;
}


void SelectionVertex::select(bool add)
{
	Selection::select(add);

	//selector->selectLevel(&level->selection, true);

	level->selection.select(true);
}


void SelectionVertex::deleteSelection()
{
	level->deleteVertex(segmentId, id);
}


void SelectionVertex::updateProperty()
{
	propertyBox.update();
}

void SelectionVertex::move(sf::Vector2f amount)
{
	(*vertex) += MATH::sfTob2(amount);
}


sf::Vector2f SelectionVertex::getPosition()
{
	return MATH::b2ToSf(*vertex);
}


std::string SelectionVertex::getName()
{
	std::stringstream ss;

	ss << id;

	return ss.str();
	
}



int SelectionVertex::getSegmentId()
{
	return segmentId;
}
int SelectionVertex::getVertexId()
{
	return id;
}

void SelectionVertex::unselect()
{

}

void SelectionVertex::setVertex(b2Vec2* _vertex, int _segmentId, int _id)
{
	id = _id;
	segmentId = _segmentId;

	vertex = _vertex;

	hierarchyButton->SetLabel(getName());

	propertyBox.setVertex(_vertex);
}


void SelectionVertex::setSegmentId(int _segmentId)
{
	segmentId = _segmentId;
}



sfg::Box::Ptr SelectionVertex::getPropertyBox()
{
	return propertyBox.get();
}