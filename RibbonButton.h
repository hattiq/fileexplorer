#pragma once
#include "Control.h"
class RibbonButton:public Control
{
public:
	enum ButtonType { NEW_FILE=0, NEW_FOLDER, DELETEE, RENAME, COPY, MOVE, PASTE,BACK,FORTH,INVALID };
	RibbonButton(ButtonType t);
	void draw(RenderTarget &target, RenderStates states) const override;
	ButtonType getType();
	virtual ~RibbonButton();
	virtual FloatRect getGlobalBounds() const;
	virtual Control * handleEvent(Event event, Vector2f mousePos);

	virtual Control * clone() const;
private:
	void loadResources();
private:
	ButtonType type;
	static Texture textures[9];
	static bool isResourcesLoaded;

};