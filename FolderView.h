#pragma once
#include <vector>
#include <string>
#include "File.h"
#include "Drive.h"

class Ribbon;

using namespace std;
class FolderView:public Control
{
public:
	FolderView(Vector2f size);

	string getCurrentPath()const;
	string getSelectedFilePath()const;
	string getSelectedFileName() const;

	void loadDirectory(const string path, bool effectRibbon=true);

	virtual ~FolderView();
	virtual FloatRect getGlobalBounds() const;
	virtual Control * handleEvent(Event event, Vector2f mousePos);

	virtual Control * clone() const;

	void attachRibbon(Ribbon * rb);

	void refreshDirectory();
	void refreshDrives();
	void refresh();

	void draw(RenderTarget &target, RenderStates states) const override;
	
	void positionDrives();
	void positionFiles();
private:
	void loadDrives();

	//Graphical
	void onClickLeft(const Event::MouseButtonEvent& mouseButton, Vector2f mousePos);
	void onBackSpace();
	void onMouseMove(const Event::MouseMoveEvent& mouse, Vector2f mousePos);
private:
	Clock lastClickTime;
	Ribbon* pRibbon;
	FloatRect globalBounds;
	string currentPath="";
	vector<Drive> drives;
	vector<File> files;
	int selectedFile=-1;
	int selectedDrive=-1;

};