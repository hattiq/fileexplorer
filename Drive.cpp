#include "Drive.h"
#include <Windows.h>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

Texture Drive::driveIcon;
bool Drive::isDriveResourcesLoaded = false;
Font Drive::driveFont;

const sf::Color Drive::flatColorConcrete(149, 165, 166);
const sf::Color Drive::flatColorMidnightBlue(41, 128, 185);

Drive::Drive(const string & driveLetter)
{
	if (!isDriveResourcesLoaded)
	{
		loadDriveResources();
		isDriveResourcesLoaded = true;
	}
	rootPath = driveLetter;
	loadNames();
	refreshStatistics();

	setBackgroundColor(Color::Transparent);
	setSize(Vector2f(300, 90));
	refreshGraphicalVariables();
}

void Drive::refreshStatistics()
{
	ULARGE_INTEGER freeDiskBytes, totalDiskBytes;
	if (GetDiskFreeSpaceEx(rootPath.c_str(), &freeDiskBytes, &totalDiskBytes, nullptr))
	{
		freeSpace = freeDiskBytes.QuadPart;
		capacity = totalDiskBytes.QuadPart;
		usedSpace = totalDiskBytes.QuadPart - freeDiskBytes.QuadPart;
	}
}

void Drive::refresh()
{
	refreshStatistics();
	refreshGraphicalVariables();
}

string Drive::getPath()
{
	return rootPath;
}

string Drive::getLabel()
{
	return label;
}

string Drive::getFileSysName()
{
	return fileSystemName;
}

unsigned long long Drive::getCapacity()
{
	return capacity;
}

unsigned long long Drive::getUsedSpace()
{
	return usedSpace;
}

unsigned long long Drive::getFreeSpace()
{
	return freeSpace;
}

void Drive::draw(RenderTarget &target, RenderStates states) const
{
	Vector2f thisPos = getPosition();

	Control::draw(target, states);

	float xSpacing = horizontalSpacing;
	float ySpacing = verticalSpacing;
	Sprite icon(driveIcon);
	icon.setPosition(thisPos);
	icon.move(xSpacing, ySpacing);

	xSpacing += 5;

	Text txtLabel(labelAndLetterTxt, driveFont, 20);
	txtLabel.setPosition(thisPos);
	txtLabel.move(icon.getLocalBounds().width + xSpacing, ySpacing);

	ySpacing += 5;

	RectangleShape rectBlue(usedSpaceRectSize);
	rectBlue.setPosition(thisPos.x + icon.getLocalBounds().width + xSpacing,
		thisPos.y + txtLabel.getLocalBounds().height + ySpacing);

	RectangleShape rectGray(freeSpaceRectSize);
	rectGray.setPosition(rectBlue.getPosition().x + rectBlue.getLocalBounds().width, rectBlue.getPosition().y);

	Text txtDrive(driveFileSystemTxt, driveFont, 13);
	txtDrive.setPosition(rectBlue.getPosition().x,
		rectBlue.getPosition().y + rectBlue.getLocalBounds().height);

	Text txtSpace(driveSpaceInfoTxt, driveFont, 15);
	txtSpace.setPosition(txtDrive.getPosition().x,
		txtDrive.getPosition().y + txtDrive.getLocalBounds().height + 5);

	txtLabel.setFillColor(Color::Black);
	txtDrive.setFillColor(Color::Black);
	txtSpace.setFillColor(Color::Black);

	rectGray.setFillColor(flatColorConcrete);
	rectBlue.setFillColor(flatColorMidnightBlue);

	target.draw(icon);
	target.draw(txtLabel);
	target.draw(rectBlue);
	target.draw(rectGray);
	target.draw(txtDrive);
	target.draw(txtSpace);

}

Drive::~Drive()
{
}

FloatRect Drive::getGlobalBounds() const
{
	return FloatRect(getPosition(),getSize());
}

Control * Drive::handleEvent(Event event, Vector2f mousePos)
{
	return this;
}

Control * Drive::clone() const
{
	return new Drive(*this);
}

bool Drive::loadNames()
{
	char volumeNameBuf[300], fileSystemNameBuf[100];
	unsigned long volumeNameBufSize = 300, fileSystemNameBufSize = 100;

	if (GetVolumeInformation(
		rootPath.c_str(),
		volumeNameBuf,
		volumeNameBufSize,
		nullptr,//serial number not required
		nullptr,//Max Component Size Not Required
		nullptr,//Flags not required
		fileSystemNameBuf,
		fileSystemNameBufSize))
	{
		label = volumeNameBuf;
		fileSystemName = fileSystemNameBuf;
		driveType = GetDriveType(rootPath.c_str());
		return true;
	}
	driveType = GetDriveType(rootPath.c_str());

	return false;
}

void Drive::loadDriveResources()
{
	driveIcon.loadFromFile("resources\\images\\drive.png");
	driveIcon.setSmooth(true);
	driveFont.loadFromFile("resources\\fonts\\Roboto-Medium.ttf");
}

void Drive::refreshGraphicalVariables()
{
	labelAndLetterTxt = ((label.length() == 0 ? String("Local Disk") : String(label)) + " (" + rootPath.substr(0, 2) + ')');
	if (label.length() > 0)
	{
		labelAndLetterTxt = label + " (" + rootPath.substr(0, 2) + ')';
	}
	else if (driveType == DRIVE_CDROM)
	{
		labelAndLetterTxt = "CD/DVD ROM (" + rootPath.substr(0, 2) + ')';
	}
	else
	{
		labelAndLetterTxt = "Local Disk (" + rootPath.substr(0, 2) + ')';
	}

	driveFileSystemTxt = fileSystemName;

	stringstream ss;
	double gbConversionDivsor = 1073741824;
	ss << setprecision(1) << fixed << freeSpace / gbConversionDivsor << '/' << capacity / gbConversionDivsor << "GB";
	driveSpaceInfoTxt = ss.str();
	float totalLength = (getSize().x - 3 * horizontalSpacing - driveIcon.getSize().x);
	freeSpaceRectSize = Vector2f(totalLength*(freeSpace / float(capacity)), 10);
	usedSpaceRectSize = Vector2f(totalLength*(usedSpace / float(capacity)), 10);
}
