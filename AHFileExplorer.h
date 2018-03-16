#pragma once

#include "Container.h"
#include "FolderView.h"
#include "TreeNode.h"
#include "Ribbon.h"

using namespace std;
using namespace sf;

class AHFileExplorer
{
public:
	AHFileExplorer();
	~AHFileExplorer();

	void run();

private:
	void populateNode(TreeNode * ptr, Texture & ft, Font & f);
	void loadDrives(TreeNode * root, Texture & ft, Font & f);
private:
	FolderView folderView;
	Ribbon ribbon;

	Texture folderIcon;
	Texture thisPcIcon;
	Texture driveIcon;
	Font font;
};