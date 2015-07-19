#ifndef PROPERTY_BOX_BODY_H
#define PROPERTY_BOX_BODY_H

#include "PropertyBox.h"
#include <Box2D\Box2D.h>


class LevelObject;

class PropertyBoxCollider;
class PropertyBoxCircleShape;
class PropertyBoxPolygonShape;



class PropertyBoxBody : public PropertyBox
{
public:
	PropertyBoxBody();

	void shutdown();

	void setObject(LevelObject* _object);

	void build();

	void update();

	void deleteCollider(int id);

private:
	LevelObject* object;

	sfg::Entry::Ptr angle;
	sfg::ComboBox::Ptr type;
	sfg::ComboBox::Ptr active;
	sfg::ComboBox::Ptr fixedRotation;
	sfg::ComboBox::Ptr bullet;
	sfg::Entry::Ptr gravityScale;

	sfg::Button::Ptr addBox;
	sfg::Button::Ptr addCircle;

	std::vector<PropertyBoxCollider*> colliders;

	void setAngle();
	void setType();
	void setActive();
	void setFixedRotation();
	void setBullet();
	void setGravityScale();

	void doAddBox();
	void doAddCircle();

};

class PropertyBoxCollider : public PropertyBox
{
public:

	void setCollider(LevelObject* _object, int id);
	void setParent(PropertyBoxBody* _parent);

	void shutdown();

	void update();
	void build();

private:

	PropertyBoxBody* parent;
	LevelObject* object;
	int colliderId;

	PropertyBox* shape;

	sfg::Entry::Ptr friction;
	sfg::Entry::Ptr restitution;
	sfg::Entry::Ptr density;
	sfg::Entry::Ptr userData;
	sfg::ComboBox::Ptr isSensor;
	sfg::Button::Ptr _delete;

	void setFriction();
	void setRestitution();
	void setDensity();
	void setUserData();
	void setSensor();

	void doDelete();
};



class PropertyBoxCircleShape : public PropertyBox
{
public:

	void setShape(b2CircleShape* _shape);
	
	void build();
	void update();

private:
	
	b2CircleShape* shape;

	sfg::Entry::Ptr positionX;
	sfg::Entry::Ptr positionY;

	sfg::Entry::Ptr radius;


	void setPosition();
	void setRadius();


};

class PropertyBoxPolygonShape : public PropertyBox
{
public:

	void setShape(b2PolygonShape* _shape);
	
	void build();
	void update();

private:
	
	b2PolygonShape* shape;

	sfg::Entry::Ptr positionX;
	sfg::Entry::Ptr positionY;

	sfg::Entry::Ptr width;
	sfg::Entry::Ptr height;

	sfg::Entry::Ptr angle;

	void set();	
};

#endif