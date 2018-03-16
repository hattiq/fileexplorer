#ifndef Tree_Node_H
#define Tree_Node_H

#include"Control.h"

class TreeNode : public Control
{
private:
	vector<TreeNode*>	childNode;
	TreeNode*			parent;
	string				name;
	Text				text;
	Sprite				icon;
	bool				isExpanded;
	FloatRect			globalBounds;

	static Texture		downArrow;
	static Texture		rightArrow;
	static bool			isTextureLoaded;

	static const int nodeHeight = 25;
public:
	TreeNode(const string & title, const string & address, const Texture & ico, const Font & f);
	TreeNode(const TreeNode & ref);
	~TreeNode();

	TreeNode*	getChildNode(int index)	const;
	TreeNode*	getParent()				const;
	string		getText()				const;
	string		getName()				const;
	int			getNoOfChilds()			const;
	int			findNodeHeight()		const;
	bool		isNodeExpanded()		const;


	void addNode(const TreeNode & node);
	void removeAllNodes();
	void setParent(TreeNode * ptr);
	void setPosition(const Vector2f & pos);
	void setIcon(const Sprite & ico);
	void setText(const string & title);
	void setName(const string & address);
	void expandNode(bool val);


	virtual FloatRect getGlobalBounds() const;
	virtual Control * clone() const;


	virtual Control * handleEvent(Event event, Vector2f mousePos);
	TreeNode * onClick(Vector2f mouse);

	virtual void draw(RenderTarget& target, RenderStates states) const;

	static void loadTextures();
};

#endif // !Tree_Node_H
