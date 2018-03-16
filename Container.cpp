#include"Container.h"

Container::Container(const FloatRect & rectangle, Control & ctrl, const Vector2f & windowSize)
	:
	bounds(rectangle),
	control(ctrl),
	containerView(FloatRect(0, 0, rectangle.width, rectangle.height)),
	controlView(FloatRect(0, 0, rectangle.width - scrollBarSize, rectangle.height)),
	scroll({ rectangle.width - scrollBarSize,0 }, { (float)scrollBarSize,rectangle.height }, controlView, { control.getGlobalBounds().width, control.getGlobalBounds().height })
{
	float ratioX = (1.0 / windowSize.x);
	float ratioY = (1.0 / windowSize.y);
	containerView.setViewport(FloatRect({ ratioX * rectangle.left , ratioY * rectangle.top, ratioX * rectangle.width,ratioY * rectangle.height }));
	controlView.setViewport(FloatRect({ ratioX * rectangle.left , ratioY * rectangle.top, ratioX * (rectangle.width - scrollBarSize),ratioY * rectangle.height }));

	control.setPosition({ 0, 0 });
}
void Container::resize(const FloatRect & rectangle, const Vector2f & windowSize)
{
	bounds = rectangle;
	containerView.reset(FloatRect(0, 0, rectangle.width, rectangle.height));
	controlView.reset(FloatRect(0, 0, rectangle.width - scrollBarSize, rectangle.height));
	float ratioX = (1.0 / windowSize.x);
	float ratioY = (1.0 / windowSize.y);
	containerView.setViewport(FloatRect({ ratioX * rectangle.left , ratioY * rectangle.top, ratioX * rectangle.width,ratioY * rectangle.height }));
	controlView.setViewport(FloatRect({ ratioX * rectangle.left , ratioY * rectangle.top, ratioX * (rectangle.width - scrollBarSize),ratioY * rectangle.height }));
	scroll.setSettings({ rectangle.width - scrollBarSize,0 }, { (float)scrollBarSize,rectangle.height });
	control.setPosition({ 0, 0 });
}
bool Container::contains(const Vector2f & point)
{
	return bounds.contains(point);
}
Control *  Container::handleEvent(RenderWindow & window, Event event, Vector2i mousePos)
{

	if (event.type == Event::MouseWheelMoved)
	{
		float wheel = event.mouseWheel.delta * -1;
		scroll.scroll(wheel);
		return nullptr;
	}

	Vector2f containerRelative = window.mapPixelToCoords(mousePos, containerView);

	if (scroll.contains(containerRelative) && event.type == Event::MouseButtonPressed)
	{
		if (scroll.onBar(containerRelative))
		{
			Window & w = window;
			Vector2f current = containerRelative;
			while (Mouse::isButtonPressed(Mouse::Button::Left))
			{
				current = window.mapPixelToCoords(Mouse::getPosition(window), containerView);
			}
			scroll.scroll((current.y - containerRelative.y) / 10);
			return nullptr;
		}
		else if (scroll.onScrollUpButton(containerRelative))
		{
			scroll.scrollUpButtonClick();
			return nullptr;
		}
		else if (scroll.onScrollDownButton(containerRelative))
		{
			scroll.scrollDownButtonClick();
			return nullptr;
		}
		return nullptr;
	}
	else
	{
		Vector2f controlRelative = window.mapPixelToCoords(mousePos, controlView);
		Control * ptr = control.handleEvent(event, controlRelative);
		return ptr;
	}
	return nullptr;
}
void Container::draw(RenderTarget& target, RenderStates states) const
{
	const View & temp = target.getView();

	control.setPosition({ 0,0 });

	target.setView(containerView);
	if (control.getGlobalBounds().height > bounds.height)
	{
		target.draw(scroll);
	}
	target.setView(controlView);
	target.draw(control);

	target.setView(temp);
}

void Container::updateControlSize(const Vector2f & size)
{
	scroll.updateTargetSize(size);
}