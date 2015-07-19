#include "PropertyBox.h"


sfg::Box::Ptr PropertyBox::get()
{
	return box;
}

void PropertyBox::update()
{

}

void PropertyBox::build()
{
	box = sfg::Box::Create(sfg::Box::VERTICAL);
}