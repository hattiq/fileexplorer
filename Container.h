#ifndef CONTAINER_H
#define CONTAINER_H


#include"ScrollBar.h"
#include"Control.h"

class Container : public Drawable
{
private:
	View controlView;
	View containerView;
	FloatRect bounds;
	Control & control;
	ScrollBar  scroll;

	static const int scrollBarSize = 15;
public:
	Container(const FloatRect & rectangle, Control & ctrl, const Vector2f & windowSize);
	void resize(const FloatRect & rectangle, const Vector2f & windowSize);
	bool contains(const Vector2f & point);
	Control * handleEvent(RenderWindow & window, Event event, Vector2i mousePos);
	virtual void draw(RenderTarget& target, RenderStates states) const;
	void updateControlSize(const Vector2f & size);
};


#endif // !CONTAINER_H
