#include "SelectionLevel.h"
#include "Selector.h"
#include "EditorLevel.h"
#include "SelectionVertex.h"
#include <iostream>

SelectionLevel::SelectionLevel()
{
	_hasToolbar = true;

	propertyBox.build();

	toolbarBox = sfg::Box::Create(sfg::Box::HORIZONTAL);
	insertButton = sfg::Button::Create("Insert Vertex");
	insertButton->GetSignal(sfg::Button::OnLeftClick).Connect(&SelectionLevel::doInsertVertex, this);
	toolbarBox->Pack(insertButton);

	breakButton = sfg::Button::Create("Break Vertex");
	breakButton->GetSignal(sfg::Button::OnLeftClick).Connect(&SelectionLevel::doBreakVertex, this);
	toolbarBox->Pack(breakButton);

	weldButton = sfg::Button::Create("Weld Vertices");
	weldButton->GetSignal(sfg::Button::OnLeftClick).Connect(&SelectionLevel::doWeldVertecies, this);
	toolbarBox->Pack(weldButton);
}



void SelectionLevel::shutdown()
{
	propertyBox.shutdown();
}

void SelectionLevel::move(sf::Vector2f amount)
{

}


void SelectionLevel::updateProperty()
{
	propertyBox.update();
}


sf::Vector2f SelectionLevel::getPosition()
{
	return sf::Vector2f(0.0f,0.0f);
}


void SelectionLevel::select(bool add)
{
	selector->selectLevel(this, add);
	hierarchyButton->SetActive(true);
}


std::string SelectionLevel::getName()
{
	return "Level";
}


void SelectionLevel::setLevel(EditorLevel& _level)
{
	level = &_level;
	propertyBox.setLevel(_level);
}

sfg::Box::Ptr SelectionLevel::getPropertyBox()
{
	return propertyBox.get();
}


void SelectionLevel::addSegment()
{
	propertyBox.addSegment(level->fixtureDefs.back());

}


void SelectionLevel::addVertex(int segmentId, sfg::Box::Ptr vertexPropertyBox)
{
	propertyBox.segments[segmentId]->get()->Pack(vertexPropertyBox, false);
}

void SelectionLevel::insertVertex(int segmentId, int position, sfg::Box::Ptr vertexPropertyBox)
{
	propertyBox.segments[segmentId]->get()->Pack(vertexPropertyBox, false);
	propertyBox.segments[segmentId]->get()->ReorderChild(vertexPropertyBox, position + 6);
}

void SelectionLevel::deleteSegment(int segmentId)
{




	propertyBox.deleteSegment(segmentId);

}


void SelectionLevel::deleteVertex(int segmentId, sfg::Box::Ptr vertexPropertyBox)
{
	propertyBox.segments[segmentId]->get()->Remove(vertexPropertyBox);
}


void SelectionLevel::doInsertVertex()
{
	SelectionVertex* vertices[2] = {NULL,NULL};

	int j = 0;

	if (selector->selection.size() > 2)
	{
		std::cout << "too many vertices selected" << std::endl;
		return; // too many vertices selected
	}


	for (int i = 0; i < selector->selection.size(); i++)
	{
		if (vertices[j] = dynamic_cast<SelectionVertex*>(selector->selection[i]))
		{
			j++;
			//if (j > 2)
			//{
			//	std::cout << "too many vertices selected" << std::endl;

			//	return; // too many vertices selected
			//}
		}
	}


	if (j <= 1)
	{
		std::cout << "too few vertices selected" << std::endl;
		return; // too few vertices selected
	}
	if (vertices[0]->getSegmentId() != vertices[1]->getSegmentId())
	{
		std::cout << "vertices from different Segments" << std::endl;
		return; // vertices from different Segments
	}

	if (vertices[0]->getVertexId()+1 != vertices[1]->getVertexId() &&
		vertices[1]->getVertexId()+1 != vertices[0]->getVertexId())
	{
		std::cout << "vertecies not adjacent" << std::endl;
		return; // vertecies not adjacent
	}

	if (vertices[0]->getVertexId() < vertices[1]->getVertexId())
	{
		level->insertVertex(*selector, vertices[0]->getVertexId(), vertices[0]->getSegmentId()); 
		return;
	}


	if (vertices[1]->getVertexId() < vertices[0]->getVertexId())
	{
		level->insertVertex(*selector, vertices[1]->getVertexId(), vertices[1]->getSegmentId()); 
		return;
	}


}

void SelectionLevel::doBreakVertex()
{
	SelectionVertex* vertex = NULL;

	int j = 0;

	if (selector->selection.size() > 1)
	{
		std::cout << "too many vertices selected" << std::endl;
		return; // too many vertices selected
	}

	for (int i = 0; i < selector->selection.size(); i++)
	{
		if (vertex = dynamic_cast<SelectionVertex*>(selector->selection[i]))
		{
			j++;

			//if (j > 1)
			//{
			//	std::cout << "too many vertices selected" << std::endl;
			//	return; // too many vertices selected
			//}
		}
	}


	if (j == 0)
	{
		std::cout << "no Vertices selected" << std::endl;
		return; // too few vertices selected
	}


	level->breakVertex(*selector, vertex->getVertexId(), vertex->getSegmentId());

}


void SelectionLevel::doWeldVertecies()
{
	
	SelectionVertex* vertices[2] = {NULL,NULL};

	int j = 0;

	if (selector->selection.size() > 2)
	{
		std::cout << "too many vertices selected" << std::endl;
		return; // too many vertices selected
	}

	for (int i = 0; i < selector->selection.size(); i++)
	{
		if (vertices[j] = dynamic_cast<SelectionVertex*>(selector->selection[i]))
		{
			j++;
			//if (j >= 2)
			//{
			//	std::cout << "too many vertices selected" << std::endl;
			//	return; // too many vertices selected
			//}
		}
	}


	if (j <= 1)
	{
		std::cout << "too few vertices selected" << std::endl;
		return; // too few vertices selected
	}
	if (vertices[0]->getSegmentId() == vertices[1]->getSegmentId())
	{
		std::cout << "vertices from same Segments" << std::endl;
		return; // vertices from different Segments
	}

	if (vertices[0]->getVertexId() != 0 &&
		vertices[0]->getVertexId() != level->selections[vertices[0]->getSegmentId()].size() - 1 &&
		vertices[1]->getVertexId() != 0 &&
		vertices[1]->getVertexId() != level->selections[vertices[1]->getSegmentId()].size() - 1 )
	{
		std::cout << "vertecies not endpoints" << std::endl;
		return; // vertecies not adjacent
	}

	selector->clearSelection();

	level->weldVertices(*selector, vertices[0]->getVertexId(), vertices[0]->getSegmentId(), vertices[1]->getVertexId(), vertices[1]->getSegmentId());



}