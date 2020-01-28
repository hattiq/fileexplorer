#pragma once
class TreeView;
#include "FileSystem.h"
#include "FolderView.h"
#include "TextBox.h"
#include "RibbonButton.h"
using namespace std;
class Ribbon:public Control
{
public:
	Ribbon(FolderView * fv, const Vector2f & si);
	void positionButtons();
	void draw(RenderTarget &target, RenderStates states) const override;
	virtual Control * handleEvent(Event event, Vector2f mousePos);

	void setPath(string path, bool truncateFollowing = false);

	void refresh();

	virtual ~Ribbon();
	virtual FloatRect getGlobalBounds() const;
	

	virtual Control * clone() const;

private:
	void onMouseLeftClick(Event::MouseButtonEvent & mouseButton);
	void onMouseOver(Event::MouseMoveEvent & mouse);
	void onButtonPress(RibbonButton::ButtonType t);
	void processInput();
private:
	vector<RibbonButton> buttons;
	vector<string> paths;
	int currentPathIndex;

	FolderView * folderView;
	FileSystem fs;
	TextBox directoryTextBox;

	string operationFrom;//for copy and move
	/*bool inputInProgress;
	string inputString;*/
	RibbonButton::ButtonType lastOperation;
};