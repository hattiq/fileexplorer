#include"ScrollBar.h"


Texture ScrollBar::caretUp;
Texture ScrollBar::caretDown;
bool ScrollBar::isTextureLoaded = false;

void ScrollBar::loadTextures()
{
	caretUp.loadFromFile("resources/images/cup.png");
	caretDown.loadFromFile("resources/images/cdown.png");
	caretUp.setSmooth(true);
	caretDown.setSmooth(true);
}
void ScrollBar::setSettings(const Vector2f & position, const Vector2f & size)
{
	scrollArea.setPosition(position);
	scrollUpButton.setPosition(position);
	scrollDownButton.setPosition({ position.x,position.y + size.y - size.x });
	bar.setPosition({ position.x,position.y + size.x });

	scrollArea.setSize(size);

	sUp.setPosition({ scrollUpButton.getPosition().x + 1 ,scrollUpButton.getPosition().y + 1 });
	sDown.setPosition({ scrollDownButton.getPosition().x + 1 ,scrollDownButton.getPosition().y + 1 });

	Vector2f scrollTotalSize = { scrollArea.getSize().x, size.y - (2 * scrollArea.getSize().x) };
	bar.setSize({ 15,(size.y / targetSize.y) * scrollTotalSize.y });
}
ScrollBar::ScrollBar(const Vector2f & position, const Vector2f & size, View & view, const Vector2f & targetViewSize) : targetView(view)
{
	if (!isTextureLoaded)
	{
		loadTextures();
		isTextureLoaded = true;
	}
	targetSize = targetViewSize;
	scrollValue = 0;

	bar.setPosition({ 0,0 });

	setSettings(position, size);

	scrollUpButton.setSize({ size.x,size.x });
	scrollDownButton.setSize({ size.x,size.x });

	scrollArea.setFillColor(Color::Color(230, 230, 230, 255));
	bar.setFillColor(Color::Color(200, 200, 200, 255));
	scrollUpButton.setFillColor(Color::Color(200, 200, 200, 255));
	scrollDownButton.setFillColor(Color::Color(200, 200, 200, 255));

	sUp.setTexture(caretUp);
	sDown.setTexture(caretDown);
	sUp.setScale({ 0.4f,0.4f });
	sDown.setScale({ 0.4f,0.4f });
}
void ScrollBar::updateTargetSize(const Vector2f & targetViewSize)
{
	Vector2f scrollTotalSize = { scrollArea.getSize().x, scrollArea.getSize().y - (2 * scrollArea.getSize().x) };
	targetSize = targetViewSize;
	int value = scrollValue;
	if (scrollArea.getSize().y >= targetSize.y)
	{
		targetView.move({ 0,float(-value) });
		scrollValue = 0;
		Vector2f scrollTotalSize = { scrollArea.getSize().x, scrollArea.getSize().y - (2 * scrollArea.getSize().x) };
		bar.setSize({ 15,(scrollArea.getSize().y / targetSize.y) * scrollTotalSize.y });
		bar.setPosition({ scrollArea.getPosition().x,scrollArea.getPosition().y + scrollUpButton.getSize().y });
	}
	else
	{
		scrollValue = 0;
		bar.setPosition({ scrollArea.getPosition().x,scrollArea.getPosition().y + scrollUpButton.getSize().y });
		targetView.move({ 0,float(-value) });
		bar.setSize({ 15,(scrollArea.getSize().y / targetSize.y) * scrollTotalSize.y });
		if (!scroll(value))
		{
			scrollValue = targetSize.y - scrollArea.getSize().y;
			targetView.move({ 0,float(scrollValue) });
			bar.setPosition({ scrollArea.getPosition().x,scrollDownButton.getPosition().y - bar.getSize().y });
		}
	}
}
bool ScrollBar::scroll(float value)
{
	float offsetY = (bar.getSize().y / scrollArea.getSize().y) * value;

	if (bar.getPosition().y + offsetY >= (scrollUpButton.getPosition().y + scrollUpButton.getSize().y) &&
		bar.getPosition().y + offsetY + bar.getSize().y <= scrollDownButton.getPosition().y)
	{
		bar.move({ 0,(bar.getSize().y / scrollArea.getSize().y) * value });
		targetView.move({ 0,value });
		scrollValue += value;
		return true;
	}
	return false;
}
void ScrollBar::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(scrollArea, states);
	target.draw(bar, states);
	target.draw(scrollUpButton, states);
	target.draw(scrollDownButton, states);
	target.draw(sUp, states);
	target.draw(sDown, states);
}
bool ScrollBar::scrollUpButtonClick()
{
	return scroll(-1);
}
bool ScrollBar::scrollDownButtonClick()
{
	return scroll(1);
}
bool ScrollBar::onScrollUpButton(const Vector2f & point)
{
	return FloatRect(scrollUpButton.getPosition(), scrollUpButton.getSize()).contains(point);
}
bool ScrollBar::onScrollDownButton(const Vector2f & point)
{
	return FloatRect(scrollDownButton.getPosition(), scrollDownButton.getSize()).contains(point);
}
bool ScrollBar::onBar(const Vector2f & point)
{
	return FloatRect(bar.getPosition(), bar.getSize()).contains(point);
}
bool ScrollBar::contains(const Vector2f & point)
{
	return FloatRect(scrollArea.getPosition(), scrollArea.getSize()).contains(point);
}