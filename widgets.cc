#include "widgets.hh"

void GUIElement::addLayout(SP<Layout> const &layout)
{
	this->layout = layout;
}

void GUIElement::addElement(SP<GUIElement> const &element)
{
	this->childElements.push_back(element);
}
