#include "Selector.h"
#include <myMath.h>
#include <Defines.h>
#include "LevelObject.h"

Selector::Selector()
{
	selecting = false;
	//selection = 0;

	//handle.setFillColor(sf::Color(0,0,255,128));
	//handle.setRadius(7.0f);
	//handle.setOrigin(7.0f, 7.0f);

	//handle.width = 10;
	//handle.height = 10;

	translating = false;

	//levelBox.build();

	levelSelection = 0;
}




void Selector::draw(sf::RenderTarget& target, Camera* camera)
{

	target.setView(camera->getView());

	if (selection.size() > 0)
	{

		sf::Vector2f handlePosition = sf::Vector2f(0.0f,0.0f);

		for (int i = 0; i < selection.size(); i++)
		{
			handlePosition += selection[i]->getPosition();
		}

		handlePosition *= 1.0f/selection.size();

		handlePosition.x *= mToPx;
		handlePosition.y *= -mToPx;

		float zoomFactor = camera->getView().getSize().x / target.getDefaultView().getSize().x;
		float size = 15.0f * zoomFactor;

		handle.width = size;
		handle.height = size;

		handle.left = handlePosition.x - size/2.0f;
		handle.top = handlePosition.y - size/2.0f;

		sf::RectangleShape rect;
		rect.setPosition(handlePosition);
		rect.setSize(sf::Vector2f(size,size));
		rect.setOrigin(size/2.0f,size/2.0f);

		rect.setFillColor(sf::Color(0,0,255,150));

	

		target.draw(rect);

	}


	if (selecting)
	{
		sf::RectangleShape rect;
		rect.setPosition(selectionRect.left * mToPx, -selectionRect.top * mToPx);
		rect.setSize(sf::Vector2f(selectionRect.width * mToPx, -selectionRect.height * mToPx));

		rect.setFillColor(sf::Color::Transparent);
		rect.setOutlineColor(sf::Color(50,50,50,255));
		rect.setOutlineThickness(3.0f);

		target.draw(rect);

	}


	if (cameraSelection)
	{
		cameraSelection->draw(target);
	}

	target.setView(target.getDefaultView());
}



void Selector::update()
{
	for (int i = 0; i < selection.size(); i++)
	{
		selection[i]->updateProperty();
	}

	if (cameraSelection)
		cameraSelection->updateProperty();
}


void Selector::checkSelection(EditorLevel& level, GOIterator* objects)
{
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		clearSelection();


	level.checkSelection(selectionRect);

	GOIterator* it = objects; 
	for(; it; it = it->next)
	{
		LevelObject* object = (LevelObject*)it->object;

		object->checkSelection(selectionRect);

	}

}

void Selector::moveSelection(sf::Vector2f& amount)
{
	for (int i = 0; i < selection.size(); i++)
	{
		selection[i]->move(amount);
	}

}

void Selector::clearSelection()
{
	for (int i = 0; i < selection.size(); i++)
	{
		if (selection[i])
			selection[i]->unSelect();
	}

	selection.clear();

	if (levelSelection)
	{
		int index = properties->GetPageOf(levelSelection->getPropertyBox());
		properties->RemovePage(index);
		
		

		toolbar->Remove(levelSelection->getToolbarBox());
		toolbar->RefreshAll();

		levelSelection->unSelect();
		levelSelection = 0;
	}

	if (cameraSelection)
	{
		int index = properties->GetPageOf(cameraSelection->getPropertyBox());
		properties->RemovePage(index);

		cameraSelection->unSelect();
		cameraSelection = 0;
	}

}


void Selector::addSelection(Selection* _selection, bool add)
{


	if (!add && !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
		clearSelection();

	selection.push_back(_selection);
	
	if (_selection->hasProperty())
	{
		properties->PrependPage(_selection->getPropertyBox(), sfg::Label::Create(_selection->getName()));
		properties->SetCurrentPage(0);
	}

	if (_selection->hasToolbar())
	{
		toolbar->Pack(_selection->getToolbarBox());
	}
}




void Selector::selectLevel(SelectionLevel* selection, bool add)
{

	
	if (!levelSelection)
	{
		if (!add && !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			clearSelection();

		//levelSelected = true;


		levelSelection = selection;

		properties->PrependPage(selection->getPropertyBox(), sfg::Label::Create(selection->getName()));
		properties->SetCurrentPage(0);

		toolbar->Pack(levelSelection->getToolbarBox(), false);
	}

}

void Selector::selectCamera(SelectionCamera* selection, bool add)
{

	
	if (!cameraSelection)
	{
		if (!add && !sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
			clearSelection();

		//levelSelected = true;


		cameraSelection = selection;

		properties->PrependPage(selection->getPropertyBox(), sfg::Label::Create(selection->getName()));
		properties->SetCurrentPage(0);

		//toolbar->Pack(levelSelection->getToolbarBox(), false);
	}

}



void Selector::deleteSelection()
{
	for (int i = 0; i < selection.size(); i++)
	{
		if (selection[i])
			selection[i]->deleteSelection();
	}
	selection.clear();

}


void Selector::unselect(Selection* _selection)
{

	if (_selection->hasProperty())
	{
		int index = properties->GetPageOf(_selection->getPropertyBox());
		properties->RemovePage(index);
	}


	if (_selection->hasToolbar())
	{
		toolbar->Remove(_selection->getToolbarBox());
	}


	int id = -1;
	for (int i = 0; i < selection.size(); i++)
	{
		if (selection[i] == _selection)
			selection[i] = 0;
	}


}