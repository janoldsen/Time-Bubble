#ifndef SELECTION_CAMERA_H
#define SELECTION_CAMERA_H

#include "Selection.h"
#include "Camera.h"
#include "PropertyBoxCamera.h"

class SelectionCamera : public Selection
{
public:
	SelectionCamera();

	void move(sf::Vector2f amount);
	sf::Vector2f getPosition();

	std::string getName();

	void updateProperty();

	void select(bool add);

	sfg::Box::Ptr getPropertyBox();

	void setCamera(Camera* _camera);
	void setLevel(Level* _level);

	void draw(sf::RenderTarget& target);


private:
	Camera* camera;
	PropertyBoxCamera propertyBox;

};

#endif