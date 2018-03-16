#include <sstream>
#include <iomanip>
#include <SFML\Graphics.hpp>
using namespace sf;

#include "File.h"
Texture File::fileIcons[2];
Font File::fileFont[2];
bool File::isResourcesLoaded = false;

File::File(const string path, const string FileName, const Type t, unsigned long long int size) :
	path(path), type(t), size(size)
{
	setSizeString();

	unsigned int extPos = FileName.find_last_of('.');
	if (extPos >= 0 && extPos < FileName.length())
	{
		name = FileName.substr(0, extPos);
		extension = FileName.substr(extPos+1);
		toUpper(extension);
	}
	else
	{
		name = FileName;
	}
	

	if (!isResourcesLoaded)
		loadResources();

	setBackgroundColor(Color::Transparent);
	horizontalSpacing = 10;
	verticalSpacing = 10;
	characterSize = 20;
	setSize(Vector2f(300, 80));
}

string File::getPath()const
{
	return path;
}

File::Type File::getType() const
{
	return type;
}

string File::getName() const
{
	return name;
}

string File::getExtention() const
{
	return extension;
}

unsigned long long int File::getSizeInBytes() const
{
	return size;
}

void File::draw(RenderTarget & target, RenderStates states) const
{
	Vector2f thisPos = getPosition();


	Control::draw(target,states);

	thisPos.x += horizontalSpacing;
	thisPos.y += verticalSpacing;

	Sprite icon;
	icon.setTexture(fileIcons[type]);
	icon.setPosition(thisPos);
	target.draw(icon);

	if (type != File::Folder)
	{
		Text ext(extension, fileFont[1], characterSize-10);
		ext.setPosition(thisPos.x+5,thisPos.y+25);
		ext.setFillColor(Color::Magenta);
		target.draw(ext);

	}

	string nameDisplay;
	if (name.length() > 16)
	{
		nameDisplay = name.substr(0, 16) + "...";
	}
	else
	{
		nameDisplay = name;
	}

	Text nameTxt(nameDisplay, fileFont[0], characterSize);
	nameTxt.setFillColor(Color::Black);
	if(type==File::Folder)
		nameTxt.setPosition(thisPos.x + icon.getGlobalBounds().width + horizontalSpacing, thisPos.y + icon.getLocalBounds().height / 4);
	else
		nameTxt.setPosition(thisPos.x + icon.getGlobalBounds().width + horizontalSpacing, thisPos.y +verticalSpacing/2);
	target.draw(nameTxt);

	Text sizeTxt(sizeString, fileFont[0], characterSize-5);
	sizeTxt.setFillColor(Color::Black);
	sizeTxt.setPosition(nameTxt.getPosition().x, nameTxt.getPosition().y + nameTxt.getLocalBounds().height+verticalSpacing/2);

	if (type != File::Folder)
	{
		target.draw(sizeTxt);
	}

}

FloatRect File::getGlobalBounds() const
{
	return FloatRect(getPosition(),getSize());
}

Control * File::handleEvent(Event event, Vector2f mousePos)
{
	return this;
}

Control * File::clone() const
{
	return new File(*this);
}

File::~File()
{
}

void File::setSizeString()
{
	unsigned long long tempSize = size;
	if (tempSize < 1024)
	{
		sizeString = to_string(tempSize) + " Bytes";
	}
	else if (tempSize < 1048576)
	{
		sizeString = to_string(tempSize / 1024) + " KB";
	}
	else if (tempSize < 1073741824)
	{
		sizeString = to_string(tempSize / 1048576) + " MB";
	}
	else
	{
		stringstream ss;
		ss << setprecision(1) << fixed << tempSize / 1073741824.0 << " GB";
		sizeString = ss.str();
	}
}

void File::loadResources()
{
	fileFont[0].loadFromFile("resources\\fonts\\arial.ttf");
	fileFont[1].loadFromFile("resources\\fonts\\Roboto-Light.ttf");

	fileIcons[File::Folder].loadFromFile("resources\\images\\folder.png");
	fileIcons[File::Other].loadFromFile("resources\\images\\file.png");


	fileIcons[File::Folder].setSmooth(true);
	fileIcons[File::Other].setSmooth(true);
}

void File::toUpper(string & s)
{
	int len = s.length();
	for (int i = 0; i < len; i++)
	{
		if (s[i] >= 'a'&&s[i] <= 'z')
		{
			s[i] -= 32;
		}
	}
}
