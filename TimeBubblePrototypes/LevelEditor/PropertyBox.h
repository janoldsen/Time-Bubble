#ifndef PROPERTY_BOX_H
#define PROPERTY_BOX_H

#include <SFGUI\SFGUI.hpp>

class PropertyBox
{
public:

	virtual ~PropertyBox(){};

	sfg::Box::Ptr get();
	virtual void build();
	virtual void update();

	
protected:
	sfg::Box::Ptr box;

};

#endif