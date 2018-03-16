#include "TextBox.h"
bool TextBox::isResourcesLoaded = false;
Font TextBox::textBoxfont;
Texture TextBox::textCursor;

void TextBox::loadResources()
{
	textBoxfont.loadFromFile("resources\\fonts\\segoeuil.ttf");
	textCursor.loadFromFile("resources\\images\\textCursor.png");
}
void TextBox::draw(RenderTarget & target, RenderStates states) const
{
	Vector2f thisPos = getPosition();

	Control::draw(target, states);
	if (isTakingInput)
	{
		RectangleShape cover(getSize());
		cover.setPosition(thisPos);
		cover.setFillColor(Color(65,252,233,90));
		target.draw(cover);

		Text txt(input, textBoxfont, 15);
		txt.setFillColor(Color::Black);
		txt.setPosition(thisPos.x + 2, thisPos.y);
		target.draw(txt);

		if (cursorInterval.getElapsedTime().asMilliseconds() %2000 < 1000)
		{
			Sprite s(textCursor);
			s.setPosition(thisPos.x + txt.getLocalBounds().width, thisPos.y);
			target.draw(s);
		}
	}
	else
	{
		Text txt((path == "") ? "This PC" : path, textBoxfont, 15);
		txt.setFillColor(Color::Black);
		txt.setPosition(thisPos.x + 2, thisPos.y);
		target.draw(txt);
	}
}
TextBox::TextBox()
{
	setSize(Vector2f(100, 20));
	if (!isResourcesLoaded)
	{
		loadResources();
		isResourcesLoaded = true;
	}
	setBorderColor(Color::Black);
	setBorderThickness(1);
	setBackgroundColor(Color::White);
	path = "";
	input = "";
	isTakingInput = false;
}

void TextBox::setPath(string str)
{
	path = str;
}

string TextBox::getPath() const
{
	return path;
}

bool TextBox::getIsTakingInput()
{
	return isTakingInput;
}

void TextBox::setIsInputInProgress(bool i)
{
	isTakingInput = i;
}

void TextBox::setInputString(string s)
{
	input = s;
}

string TextBox::getInputString()
{
	return input;
}

TextBox::~TextBox()
{
}

FloatRect TextBox::getGlobalBounds() const
{
	return FloatRect(getPosition(), getSize());
}

Control * TextBox::handleEvent(Event event, Vector2f mousePos)
{
	if (event.type == Event::EventType::TextEntered)
	{
		if (event.text.unicode != 13)
		{
			if (event.text.unicode == '\b' && input != "")
			{
				input.pop_back();
			}
			else if(event.text.unicode != '\b')
			{
				input.push_back(event.text.unicode);
			}
		}
		else
		{
			isTakingInput = false;
		}
	}
	else if (event.type == Event::MouseButtonPressed)
	{
		if (!contains(mousePos))
		{
			isTakingInput = false;
			input = "";
		}
	}
	return this;
}

Control * TextBox::clone() const
{
	return new TextBox(*this);
}
