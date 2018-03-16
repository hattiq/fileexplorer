
#ifndef SCROLL_BAR_H
#define SCROLL_BAR_H


#include"SFML\Graphics.hpp"
#include<iostream>
#include<fstream>
#include<vector>
#include<string>

using namespace std;
using namespace sf;


class ScrollBar : public Drawable
{
private:
	RectangleShape scrollArea;
	RectangleShape bar;
	RectangleShape scrollUpButton;
	RectangleShape scrollDownButton;
	Sprite sUp;
	Sprite sDown;
	int scrollValue;

	Vector2f targetSize;
	View & targetView;

	static Texture caretUp;
	static Texture caretDown;
	static bool isTextureLoaded;

	static void loadTextures();
public:
	ScrollBar(const Vector2f & position, const Vector2f & size, View & view, const Vector2f & targetViewSize);
	void setSettings(const Vector2f & position, const Vector2f & size);
	
	void updateTargetSize(const Vector2f & targetViewSize);
	bool scrollUpButtonClick();
	bool scrollDownButtonClick();
	bool onScrollUpButton(const Vector2f & point);
	bool onScrollDownButton(const Vector2f & point);
	bool onBar(const Vector2f & point);
	bool scroll(float value);

	bool contains(const Vector2f & point);

	virtual void draw(RenderTarget& target, RenderStates states) const;
};

#endif // !SCROLL_BAR_H