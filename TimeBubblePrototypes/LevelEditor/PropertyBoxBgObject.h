#ifndef PROPERTY_BOX_BG_OBJECT_H
#define PROPERTY_BOX_BG_OBJECT_H

#include "PropertyBox.h"

class LevelEditor;
class BackGroundObject;

class PropertyBoxBgObject : public PropertyBox
{
public:
	void build(BackGroundObject* _object, LevelEditor* _editor);

	void update();
	sfg::Box::Ptr get();

private:
	BackGroundObject* object;
	LevelEditor* editor;

	sfg::Entry::Ptr name;
	sfg::Entry::Ptr positionX;
	sfg::Entry::Ptr positionY;

	sfg::Entry::Ptr scaleX;
	sfg::Entry::Ptr scaleY;

	sfg::Entry::Ptr tileableX;
	sfg::Entry::Ptr tileableY;


	sfg::Entry::Ptr rotation;

	sfg::Entry::Ptr sprite;

	sfg::Entry::Ptr depth;


	void setObjectPosition();
	void setObjectName();
	void setSprite();
	void setDepth();
	void setScale();
	void setTileable();
	void setRotation();

};

#endif