#include "SelectionCamera.h"
#include "Selector.h"
#include "Camera.h"

SelectionCamera::SelectionCamera(void)
{
	_hasProperty = true;
	_hasToolbar = false;

	propertyBox.build();

}

void SelectionCamera::move(sf::Vector2f amount)
{
}

sf::Vector2f SelectionCamera::getPosition()
{
	return camera->getPosition();
}


std::string SelectionCamera::getName()
{
	return "Camera";
}

void SelectionCamera::select(bool add)
{
	selector->selectCamera(this, add);
}


void SelectionCamera::setCamera(Camera* _camera)
{
	camera = _camera;
	propertyBox.setCamera(_camera);
}

void SelectionCamera::setLevel(Level* _level)
{
	propertyBox.setLevel(_level);
}


sfg::Box::Ptr SelectionCamera::getPropertyBox()
{
	return propertyBox.get();
}



void SelectionCamera::draw(sf::RenderTarget& target)
{
	sf::RectangleShape viewPort;

	//viewPort.setSize(camera->getView().getSize());
	viewPort.setSize(sf::Vector2f(800.0f * camera->getZoom(), 600.0f * camera->getZoom()));
	viewPort.setOrigin(viewPort.getSize().x / 2.0f, viewPort.getSize().y / 2.0f);
	viewPort.setPosition(camera->getPosition());

	viewPort.setFillColor(sf::Color::Transparent);
	viewPort.setOutlineThickness(9);
	viewPort.setOutlineColor(sf::Color::Yellow);

	target.draw(viewPort);

}


void SelectionCamera::updateProperty()
{
	propertyBox.update();
}