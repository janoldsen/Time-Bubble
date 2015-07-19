#include "Selection.h"
#include "Selector.h"



Selection::Selection()
{
	hierarchyButton = sfg::ToggleButton::Create();
	//hierarchyButton->SetActive(true);
	_hasProperty = false;
	_hasToolbar = false;

	built = false;

}

void Selection::select()
{
	//selector->addSelection(this);
	//hierarchyButton->SetActive(true);

	select(false);
}


void Selection::select(bool add)
{
	selector->addSelection(this, add);
	hierarchyButton->SetActive(true);
}


void Selection::unSelect()
{
	//sfg::Widget::State debug = hierarchyButton->GetState();

	//hierarchyButton->SetState(sfg::Widget::State::NORMAL);

	hierarchyButton->SetActive(false);
	selector->unselect(this);
}


void Selection::set(Selector* _selector, sfg::Box::Ptr* hierarchy)
{
	selector = _selector;


	hierarchyButton->SetLabel(getName());
	hierarchyButton->GetSignal(sfg::ToggleButton::OnLeftClick).Connect(&Selection::select, this);

	(*hierarchy)->Pack(hierarchyButton);

	built = true;

}

bool Selection::hasProperty()
{
	return _hasProperty;
}

bool Selection::hasToolbar()
{
	return _hasToolbar;
}

sfg::Box::Ptr Selection::getPropertyBox()
{
	return propertyBox;
}

sfg::Box::Ptr Selection::getToolbarBox()
{
	return toolbarBox;
}


void Selection::updateProperty()
{

}

void Selection::deleteSelection()
{

}


void Selection::updateHierarchyButton()
{
	hierarchyButton->SetLabel(getName());
}


bool Selection::isBuilt()
{
	return built;
}