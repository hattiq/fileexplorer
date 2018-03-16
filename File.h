#pragma once
#include <string>
#include "Control.h"
using namespace std;
class File:public Control
{
public:
	enum Type {Folder=0, Other};
	File(const string path,const string FileName, const Type t,unsigned long long int siz=0 );
	string getPath()const;
	Type getType()const;
	string getName() const;
	string getExtention()const;
	unsigned long long int getSizeInBytes() const;

	void draw(RenderTarget& target, RenderStates states) const override;

	virtual FloatRect getGlobalBounds() const;
	virtual Control * handleEvent(Event event, Vector2f mousePos);

	virtual Control * clone() const;

	~File();
private:
	void setSizeString();
	void loadResources();
	void toUpper(string & s);
private:
	//Graphical variales
	string sizeString;
	static Texture fileIcons[2];
	static Font fileFont[2];
	static bool isResourcesLoaded;
	int horizontalSpacing;
	int verticalSpacing;
	int characterSize;
	//non graphical
	Type type;
	string path;
	string name;
	string extension;
	unsigned long long int size;
};