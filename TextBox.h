#pragma once
#include "Control.h"

class TextBox : public Control
{
private:
	string path;
	string input;
	bool isTakingInput;

	Clock cursorInterval;

	static Font textBoxfont;
	static Texture textCursor;
	void loadResources();
	static bool isResourcesLoaded;
public:
	void draw(RenderTarget & target, RenderStates states)const override;

	TextBox();
	void setPath(string str);
	string getPath() const;

	bool getIsTakingInput();
	void setIsInputInProgress(bool i);
	void setInputString(string s);
	string getInputString();

	virtual ~TextBox();
	virtual FloatRect getGlobalBounds() const;
	virtual Control * handleEvent(Event event, Vector2f mousePos);
	virtual Control * clone() const;
};
