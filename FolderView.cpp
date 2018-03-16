#include "FolderView.h"
#include <Windows.h>
#include <sstream>
#include <iostream>
#include <iomanip>
#include "Ribbon.h"
using namespace std;
FolderView::FolderView(Vector2f size)
{
	globalBounds = { {0,0}, size };
	setSize(size);
	loadDrives();
	positionDrives();
	lastClickTime.restart();
	pRibbon = nullptr;
}

string FolderView::getCurrentPath() const
{
	return currentPath;
}

string FolderView::getSelectedFilePath() const
{
	if (selectedFile != -1)
	{
		return files[selectedFile].getPath();
	}
	return "";
}

string FolderView::getSelectedFileName() const
{
	if (selectedFile != -1)
	{
		return files[selectedFile].getName();
	}
	return "";
}

void FolderView::loadDirectory(const string path, bool effectRibbon)
{
	selectedDrive = -1;
	selectedFile = -1;
	if (path == "")
	{
		currentPath = "";
		if (effectRibbon)
			pRibbon->setPath(currentPath, effectRibbon);
		return;
	}

	unsigned long attribute = GetFileAttributes(path.c_str());
	if (attribute&FILE_ATTRIBUTE_DIRECTORY)
	{
		files.clear();

		currentPath = path;
		WIN32_FIND_DATA findFileData;
		void * findFileHandel;
		findFileHandel = FindFirstFile((currentPath + '*').c_str(), &findFileData);

		do
		{
			if ((findFileData.dwFileAttributes&FILE_ATTRIBUTE_HIDDEN) == 0 && findFileData.cFileName[0] != '.')
			{

				bool isDirectory = bool(findFileData.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY);
				string filePath;
				if (isDirectory)
				{
					filePath = path + findFileData.cFileName + '\\';
				}
				else
				{
					filePath = path + findFileData.cFileName;
				}

				files.emplace_back(File(filePath,
					findFileData.cFileName,
					(isDirectory ? File::Folder : File::Other),
					(isDirectory ? 0 : ((findFileData.nFileSizeHigh*(0x100000000)) + findFileData.nFileSizeLow))
					));

			}

		} while (FindNextFile(findFileHandel, &findFileData));
		FindClose(findFileHandel);
		positionFiles();
		if (effectRibbon)
			pRibbon->setPath(currentPath, effectRibbon);
	}
}

FolderView::~FolderView()
{
}

FloatRect FolderView::getGlobalBounds() const
{
	return globalBounds;
}

Control * FolderView::handleEvent(Event evnt, Vector2f mousePos)
{
	switch (evnt.type)
	{
	case Event::MouseButtonPressed:
	{
		if (evnt.mouseButton.button == Mouse::Left)
		{
			onClickLeft(evnt.mouseButton, mousePos);
		}
		break;
	}
	case Event::KeyPressed:
	{
		if (evnt.key.code == Keyboard::BackSpace)
		{
			onBackSpace();
		}
		break;
	}
	case Event::MouseMoved:
	{
		onMouseMove(evnt.mouseMove,mousePos);
		break;
	}
	case Event::Resized:
	{
		positionDrives();
		positionFiles();
		break;
	}
	}
	return this;
}

Control * FolderView::clone() const
{
	return new FolderView(*this);
}

void FolderView::attachRibbon(Ribbon * rb)
{
	pRibbon = rb;
}

void FolderView::refreshDirectory()
{
	loadDirectory(currentPath, false);
}

void FolderView::refreshDrives()
{
	loadDrives();
}

void FolderView::refresh()
{
	loadDirectory(currentPath, false);
	loadDrives();
}




void FolderView::draw(RenderTarget & target, RenderStates states) const
{

	Control::draw(target, states);
	if (currentPath == "")
	{
		for (auto &x : drives)
		{
			target.draw(x);
		}
	}
	else
	{
		for (auto &x : files)
		{
			target.draw(x);
		}
	}
}

void FolderView::loadDrives()
{
	drives.clear();
	drives.reserve(10);

	char drivesBuf[200];
	unsigned long int bufSize = 200;
	unsigned long int stringsSize = GetLogicalDriveStrings(bufSize, drivesBuf);

	unsigned long int pos = 0;
	char tempBuf[50];
	int tempPos = 0;

	while (pos < stringsSize)
	{
		tempBuf[tempPos] = drivesBuf[pos];
		if (tempBuf[tempPos] == '\0')
		{
			drives.emplace_back(Drive(tempBuf));
			tempPos = 0;
		}
		else
		{
			++tempPos;
		}
		++pos;
	}
}

void FolderView::onClickLeft(const Event::MouseButtonEvent & , Vector2f mousePos)
{
	bool interactedWithObject = false;

	if (currentPath == "")
	{
		int derivesCount = drives.size();
		for (int i = 0; i < derivesCount; i++)
		{
			if (drives[i].contains(mousePos))
			{
				interactedWithObject = true;
				if (i != selectedDrive)
				{
					if (selectedDrive != -1)
						drives[selectedDrive].setBackgroundColor(Color::Transparent);
					selectedDrive = i;
					drives[i].setBackgroundColor(Color(51, 198, 84, 200));
					lastClickTime.restart();
				}
				else if (lastClickTime.getElapsedTime().asSeconds() < 3)
				{
					drives[selectedDrive].setBackgroundColor(Color::Transparent);
					selectedDrive = -1;
					loadDirectory(drives[i].getPath());
					return;
				}
			}
		}
		if (!interactedWithObject)
		{
			if (selectedDrive != -1)
				drives[selectedDrive].setBackgroundColor(Color::Transparent);
			selectedDrive = -1;
		}
	}
	else
	{
		int filesCount = files.size();
		for (int i = 0; i < filesCount; i++)
		{
			if (files[i].contains(mousePos))
			{
				interactedWithObject = true;
				if (i != selectedFile)
				{
					if (selectedFile != -1)
						files[selectedFile].setBackgroundColor(Color::Transparent);
					selectedFile = i;
					files[selectedFile].setBackgroundColor(Color(51, 198, 84, 200));
					lastClickTime.restart();
				}
				else if (files[i].getType() == File::Folder && lastClickTime.getElapsedTime().asSeconds() < 3)
				{
					files[selectedFile].setBackgroundColor(Color::Transparent);
					selectedFile = -1;
					loadDirectory(files[i].getPath());
					return;
				}
				else if (files[i].getType() == File::Other && lastClickTime.getElapsedTime().asSeconds() < 3)
				{
					system(("\"" + files[i].getPath() + "\"").c_str());
					return;
				}
			}
		}
		if (!interactedWithObject)
		{
			if (selectedFile != -1)
				files[selectedFile].setBackgroundColor(Color::Transparent);
			selectedFile = -1;
		}
	}
}

void FolderView::onBackSpace()
{
	string newPath = currentPath.substr(0, currentPath.find_last_of('\\', currentPath.length() - 2) + 1);
	loadDirectory(newPath);
}

void FolderView::onMouseMove(const Event::MouseMoveEvent& mouse, Vector2f mousePos)
{
	if (currentPath == "")
	{
		int len = drives.size();
		for (int i = 0; i < len; i++)
		{
			if (selectedDrive == i)
			{
				drives[i].setBackgroundColor(Color(51,198,84,200));
			}
			else if (drives[i].contains(mousePos))
			{
				drives[i].setBackgroundColor(Color(51,190,198,150));
			}
			else
			{
				drives[i].setBackgroundColor(Color::Transparent);
			}
		}
	}
	else
	{
		int len = files.size();
		for (int i = 0; i < len; i++)
		{
			if (selectedFile == i)
			{
				files[i].setBackgroundColor(Color(51, 198, 84, 200));
			}
			else if (files[i].contains(mousePos))
			{
				files[i].setBackgroundColor(Color(51, 190, 198, 150));
			}
			else
			{
				files[i].setBackgroundColor(Color::Transparent);
			}
		}
	}
}

void FolderView::positionDrives()
{
	Vector2f pos(getPosition());

	globalBounds.height = 0;
	
	for (auto &d : drives)
	{
		if (pos.x + d.getSize().x < getSize().x)
		{
			d.setPosition(pos);
			pos.x += d.getSize().x;
		}
		else
		{
			pos.x = getPosition().x;
			pos.y += d.getSize().y;

			globalBounds.height += 110;

			d.setPosition(pos);
			pos.x += d.getSize().x;
		}
		d.refresh();
	}
	if (Control::getSize().y < globalBounds.height)
		Control::setSize({ Control::getSize().x,globalBounds.height });
}

void FolderView::positionFiles()
{
	Vector2f pos(getPosition());

	globalBounds.height = 0;

	for (auto &f : files)
	{
		if (pos.x + f.getSize().x < getSize().x)
		{
			f.setPosition(pos);
			pos.x += f.getSize().x;
		}
		else
		{
			pos.x = getPosition().x;
			pos.y += f.getSize().y;

			globalBounds.height += 100;

			f.setPosition(pos);
			pos.x += f.getSize().x;
		}
	}

	if (Control::getSize().y < globalBounds.height)
		Control::setSize({ Control::getSize().x,globalBounds.height });
}
