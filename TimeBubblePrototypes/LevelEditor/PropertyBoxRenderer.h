#ifndef PROPERTY_BOX_RENDERER_H
#define PROPERTY_BOX_RENDERER_H

#include "PropertyBox.h"
#include <SpriteComponent.h>
#include <AnimationComponent.h>
#include <SCMLComponent.h>

class PropertyBoxRenderer : public PropertyBox
{
public:
	PropertyBoxRenderer();

	void setRendererComponent(SpriteComponent* _renderer);

	void build();
	void update();

private:
	
	SpriteComponent* renderer;
			
	sfg::Entry::Ptr fileName;
	sfg::Entry::Ptr originX;
	sfg::Entry::Ptr originY;
	sfg::Entry::Ptr scaleX;
	sfg::Entry::Ptr scaleY;

	void setFileName();
	void setOrigin();
	void setScale();

};




class PropertyBoxAnimation : public PropertyBox
{
public:

	void setRendererComponent(AnimationComponent* _renderer);

	void build();
	void update();

private:
	AnimationComponent* renderer;

};


class PropertyBoxScml : public PropertyBox
{
public:

	void setRendererComponent(SCMLComponent* _renderer);

	void build();
	void update();

private:
	
	SCMLComponent* renderer;
			
	sfg::Entry::Ptr fileName;
	sfg::Entry::Ptr scale;
	
	sfg::ComboBox::Ptr orientation;

	void setFileName();
	void setScale();
	void setOrientation();
};

#endif