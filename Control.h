#ifndef CONTROL_H
#define CONTROL_H

#include"SFML\Graphics.hpp"
#include<iostream>
#include<vector>
#include<string>

using namespace std;
using namespace sf;

class Control : public Drawable
{
private:

	RectangleShape bounds;
public:
	Control();
	Control(const Vector2f & position, const Vector2f & size);
	virtual ~Control() = 0;
	bool contains(const Vector2f & point) const;

	virtual Vector2f getPosition() const;
	virtual Vector2f getSize() const;
	virtual FloatRect getGlobalBounds() const = 0;

	virtual void setSize(const Vector2f & size);
	virtual void setPosition(const Vector2f & position);
	
	void setBackgroundColor(Color bkColor);
	void setBorderColor(Color borderColor);
	void setBorderThickness(float thickness);

	virtual Control * handleEvent(Event event, Vector2f mousePos) = 0;

	virtual Control * clone() const = 0;

	virtual void draw(RenderTarget& target, RenderStates states) const;
};

#endif // !CONTROL_H
