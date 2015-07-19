#ifndef PROPERTY_BOX_OBJECT_H
#define PROPERTY_BOX_OBJECT_H

#include "PropertyBox.h"
#include "GameObject.h"
#include "PropertyBoxBody.h"

class LevelObject;
class PropertyBoxAi;
class PropertyBoxRenderer;

class PropertyBoxObject : public PropertyBox
{
public:
	
	~PropertyBoxObject();

	void shutdown();

	void update();
	

	void build();

	void setObject(LevelObject* _object);

private:
	LevelObject* object;

	PropertyBoxAi* aiBox;
	PropertyBox* rendererBox;
	PropertyBoxBody bodyBox;


	sfg::Box::Ptr aBox;
	sfg::ComboBox::Ptr ais;
	sfg::Button::Ptr deleteAi;

	sfg::Box::Ptr rBox;
	sfg::ComboBox::Ptr renderers;
	sfg::Button::Ptr deleteRenderer;

	sfg::Box::Ptr bBox;
	sfg::Button::Ptr addBody;
	sfg::Button::Ptr deleteBody;


	sfg::Entry::Ptr name;
	sfg::Entry::Ptr positionX;
	sfg::Entry::Ptr positionY;
	sfg::Entry::Ptr centerX;
	sfg::Entry::Ptr centerY;
	sfg::ComboBox::Ptr orientation;

	void setObjectCenter(); 
	void setObjectPosition();
	void setObjectName();
	void setObjectOrientation();

	void addAi();
	void addAi(ComponentTypes type);
	void doDeleteAi();

	void addRenderer();
	void addRenderer(ComponentTypes type);
	void doDeleteRenderer();

	void doAddBody();
	void doDeleteBody();

};



#endif