#ifndef PROPERTY_BOX_CAMERA_H
#define PROPERTY_BOX_CAMERA_H

#include "PropertyBox.h"
#include "Camera.h"

class PropertyBoxCamera : public PropertyBox
{
public:
	void update();
	void build();

	void setCamera(Camera* _camera);
	void setLevel(Level* _level);

private:
	Camera* camera;
	Level* level;

	sfg::Entry::Ptr zoom;

	sfg::Scrollbar::Ptr positionX;
	sfg::Scrollbar::Ptr positionY;

	sfg::CheckButton::Ptr depthEnabled;

	sfg::Entry::Ptr colorR;
	sfg::Entry::Ptr colorG;
	sfg::Entry::Ptr colorB;

	sfg::Entry::Ptr strength;

	sfg::CheckButton::Ptr clouds;

	sfg::Entry::Ptr sunStrength;

	void setZoom();
	void setPosition();
	void toggleDepth();
	void setColor();
	void setStrength();
	void setClouds();
	void setSunStrength();

};


#endif