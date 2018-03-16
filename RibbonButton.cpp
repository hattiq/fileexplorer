#include "RibbonButton.h"
Texture RibbonButton::textures[9];
bool RibbonButton::isResourcesLoaded = false;
RibbonButton::RibbonButton(ButtonType t)
{
	type = t;
	if (!isResourcesLoaded)
	{
		loadResources();
		isResourcesLoaded = true;
	}
	setBackgroundColor(Color::Transparent);
	setSize(Vector2f(70, 70));
}
void RibbonButton::draw(RenderTarget &target, RenderStates states) const
{
	Vector2f thisPos = getPosition();

	Control::draw(target,states);

	Sprite icon;
	icon.setTexture(textures[type]);
	icon.setPosition(thisPos.x, thisPos.y);
	target.draw(icon);

}
void RibbonButton::loadResources()
{
	textures[NEW_FILE].loadFromFile("resources\\images\\newFileButton.png");
	textures[NEW_FOLDER].loadFromFile("resources\\images\\newFolderButton.png");
	textures[DELETEE].loadFromFile("resources\\images\\deleteButton.png");
	textures[RENAME].loadFromFile("resources\\images\\renameButton.png");
	textures[COPY].loadFromFile("resources\\images\\copyButton.png");
	textures[MOVE].loadFromFile("resources\\images\\cutButton.png");
	textures[PASTE].loadFromFile("resources\\images\\pasteButton.png");
	textures[BACK].loadFromFile("resources\\images\\backButton.png");
	textures[FORTH].loadFromFile("resources\\images\\forthButton.png");
}

RibbonButton::ButtonType RibbonButton::getType()
{
	return type;
}

RibbonButton::~RibbonButton()
{
}

FloatRect RibbonButton::getGlobalBounds() const
{
	return FloatRect(getPosition(),getSize());
}

Control * RibbonButton::handleEvent(Event event, Vector2f mousePos)
{
	return this;
}

Control * RibbonButton::clone() const
{
	return new RibbonButton(*this);
}
