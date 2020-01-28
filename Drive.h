#pragma once
#include <string>
#include <SFML\Graphics.hpp>
using namespace sf;
using namespace std;

#include "Control.h"

class Drive:public Control
{
public:
	Drive(const string &driveLetter);
	
	void refresh();

	string getPath();
	string getLabel();
	string getFileSysName();
	unsigned long long getCapacity();
	unsigned long long getUsedSpace();
	unsigned long long getFreeSpace();

	void draw(RenderTarget &target, RenderStates states) const override;

	virtual FloatRect getGlobalBounds() const;
	virtual Control * handleEvent(Event event, Vector2f mousePos);

	virtual Control * clone() const;

	~Drive();
private:
	//Graphical

	void loadDriveResources();
	void refreshGraphicalVariables();

	//Non Graphical
	bool loadNames();//load label, drive letter etc
	void refreshStatistics();
private:
	//Graphical
	static bool isDriveResourcesLoaded;
	static Texture driveIcon;
	static Font driveFont;
	
	//Graphical Elements Variables
	string labelAndLetterTxt;
	string driveFileSystemTxt;
	string driveSpaceInfoTxt;
	Vector2f freeSpaceRectSize;
	Vector2f usedSpaceRectSize;
	float verticalSpacing=10;
	float horizontalSpacing = 10;

	static const sf::Color flatColorConcrete;
	static const sf::Color flatColorMidnightBlue;


	//Non Graphical 
	unsigned int driveType;
	string rootPath="";
	string label="";
	string fileSystemName="";
	unsigned long long capacity = 0;
	unsigned long long usedSpace = 0;
	unsigned long long freeSpace = 0;
};