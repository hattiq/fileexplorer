#include"Control.h"

Control::Control()
{
}
Control::Control(const Vector2f & position, const Vector2f & size) : bounds(size)
{
	bounds.setPosition(position);
}

bool Control::contains(const Vector2f & point) const
{
	return FloatRect(bounds.getGlobalBounds()).contains(point);
}
Vector2f Control::getPosition() const
{
	return bounds.getPosition();
}
Vector2f Control::getSize() const
{
	return bounds.getSize();
}
void Control::setSize(const Vector2f & size)
{
	bounds.setSize(size);
}
void Control::setPosition(const Vector2f & position)
{
	bounds.setPosition(position);
}
void Control::setBackgroundColor(Color bkColor)
{
	bounds.setFillColor(bkColor);
}
void Control::setBorderColor(Color borderColor)
{
	bounds.setOutlineColor(borderColor);
}
void Control::setBorderThickness(float thickness)
{
	bounds.setOutlineThickness(thickness);
}
void Control::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(bounds, states);
}
Control::~Control()
{}
