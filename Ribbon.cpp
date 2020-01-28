#include "Ribbon.h"
#include <iostream>

using namespace std;


Ribbon::Ribbon(FolderView *fv, const Vector2f & si)
{
	buttons.emplace_back(RibbonButton(RibbonButton::NEW_FILE));
	buttons.emplace_back(RibbonButton(RibbonButton::NEW_FOLDER));
	buttons.emplace_back(RibbonButton(RibbonButton::DELETEE));
	buttons.emplace_back(RibbonButton(RibbonButton::RENAME));
	buttons.emplace_back(RibbonButton(RibbonButton::COPY));
	buttons.emplace_back(RibbonButton(RibbonButton::MOVE));
	buttons.emplace_back(RibbonButton(RibbonButton::PASTE));
	buttons.emplace_back(RibbonButton(RibbonButton::BACK));
	buttons.emplace_back(RibbonButton(RibbonButton::FORTH));
	setSize(si);
	positionButtons();
	folderView = fv;
	paths.push_back("");
	currentPathIndex = 0;
	lastOperation = RibbonButton::INVALID;
}

void Ribbon::positionButtons()
{
	Vector2f pos(getPosition());
	for (int i = 0; i < 7; i++)
	{
		if (pos.x + buttons[i].getSize().x < getSize().x)
		{
			buttons[i].setPosition(pos);
			pos.x += buttons[i].getSize().x;
		}
		else
		{
			pos.x = getPosition().x;
			pos.y += buttons[i].getSize().y;

			buttons[i].setPosition(pos);
			pos.x += buttons[i].getSize().x;
		}
	}
	buttons[7].setPosition(Vector2f(5, 75));
	buttons[8].setPosition(Vector2f(25, 75));
	buttons[7].setSize(Vector2f(15, 20));
	buttons[8].setSize(Vector2f(15, 20));
	directoryTextBox.setPosition(Vector2f(50, 75));
	directoryTextBox.setSize(Vector2f(getSize().x - 60, 20));
}

void Ribbon::draw(RenderTarget & target, RenderStates states) const
{
	for (auto& x : buttons)
	{
		target.draw(x);
	}
	target.draw(directoryTextBox);
}

Control * Ribbon::handleEvent(Event evnt, Vector2f mousePos)
{
	switch (evnt.type)
	{
	case Event::MouseButtonPressed:
	{
		directoryTextBox.handleEvent(evnt, mousePos);
		if (evnt.mouseButton.button == Mouse::Button::Left)
		{
			onMouseLeftClick(evnt.mouseButton);
		}
		break;
	}
	case Event::MouseMoved:
	{
		onMouseOver(evnt.mouseMove);
		break;
	}
	case Event::TextEntered:
	{
		if (directoryTextBox.getIsTakingInput())
		{
			directoryTextBox.handleEvent(evnt, mousePos);
			if (!directoryTextBox.getIsTakingInput())
			{
				processInput();
			}
		}
	}
	default:
		break;
	}
	return this;
}

void Ribbon::setPath(string path, bool truncateFollowing)
{
	if (truncateFollowing && currentPathIndex != paths.size() - 1 && paths.size() > 1)
	{
		paths.erase(paths.begin() + currentPathIndex + 1, paths.end());
		paths.push_back(path);
		currentPathIndex++;
		refresh();
		return;
	}
	if (path != paths[currentPathIndex])
	{
		paths.push_back(path);
		currentPathIndex++;
		refresh();
	}
	return;
}

void Ribbon::refresh()
{
	folderView->refreshDirectory();
	directoryTextBox.setPath(paths[currentPathIndex]);
}

Ribbon::~Ribbon()
{
}

FloatRect Ribbon::getGlobalBounds() const
{
	return FloatRect(getPosition(), getSize());
}

Control * Ribbon::clone() const
{
	return new Ribbon(*this);
}


void Ribbon::onMouseLeftClick(Event::MouseButtonEvent & mouseButton)
{
	for (auto& x : buttons)
	{
		if (x.contains(Vector2f(mouseButton.x, mouseButton.y)))
		{
			onButtonPress(x.getType());
			return;
		}
	}
	if (directoryTextBox.contains(Vector2f(mouseButton.x, mouseButton.y)))
	{
		directoryTextBox.setIsInputInProgress(true);
		lastOperation = RibbonButton::INVALID;
		directoryTextBox.setInputString(folderView->getCurrentPath());
	}
}

void Ribbon::onMouseOver(Event::MouseMoveEvent & mouse)
{
	for (auto& x : buttons)
	{
		if (x.contains(Vector2f(mouse.x, mouse.y)))
		{
			x.setBackgroundColor(Color(51, 190, 198, 150));
		}
		else
		{
			x.setBackgroundColor(Color::Transparent);
		}
	}
}

void Ribbon::onButtonPress(RibbonButton::ButtonType t)
{
	switch (t)
	{
	case RibbonButton::NEW_FILE:
	{
		lastOperation = RibbonButton::NEW_FILE;
		directoryTextBox.setIsInputInProgress(true);
		directoryTextBox.setInputString("");
		/*cout << "Enter New File Name (Default: New File.txt): ";
		getline(cin,inputString);
		if (inputString == "")
		{
			inputString = "New File.txt";
		}
		if (fs.newFile(folderView->getCurrentPath(), inputString))
		{
			cout << "File Created!\n";
		}
		else
		{
			cout << "OperationFailed!\n";
		}
		inputString = "";
		folderView->refreshDirectory();*/
		break;
	}
	case RibbonButton::NEW_FOLDER:
	{
		/*cout << "Enter New Folder Name (Default: New Folder): ";
		getline(cin, inputString);
		if (inputString == "")
		{
			inputString = "New Folder";
		}
		if (fs.newFolder(folderView->getCurrentPath(), inputString))
		{
			cout << "Folder Created!\n";
		}
		else
		{
			cout << "OperationFailed!\n";
		}
		inputString = "";
		folderView->refreshDirectory();*/
		lastOperation = RibbonButton::NEW_FOLDER;
		directoryTextBox.setIsInputInProgress(true);
		directoryTextBox.setInputString("");

		break;
	}
	case RibbonButton::DELETEE:
	{
		string fileToDelete = folderView->getSelectedFilePath();
		if (fileToDelete != "" && fs.remove(fileToDelete))
		{
			folderView->refreshDirectory();
			//success
		}
		break;
	}
	case RibbonButton::RENAME:
	{
		/*cout << "Enter New Name for \"" << folderView->getSelectedFileName() << "\" : ";
		getline(cin, inputString);
		if (inputString!="" && folderView->getSelectedFilePath()!=""&& fs.rename(folderView->getSelectedFilePath(), inputString))
		{
			cout << "Rename Successful!\n";
		}
		else
		{
			cout << "OperationFailed!\n";
		}
		inputString = "";
		folderView->refreshDirectory();*/
		lastOperation = RibbonButton::RENAME;
		directoryTextBox.setIsInputInProgress(true);
		directoryTextBox.setInputString("");


		break;
	}
	case RibbonButton::COPY:
	{
		operationFrom = folderView->getSelectedFilePath();
		lastOperation = RibbonButton::ButtonType::COPY;
		break;
	}
	case RibbonButton::MOVE:
	{
		operationFrom = folderView->getSelectedFilePath();
		lastOperation = RibbonButton::ButtonType::MOVE;
		break;
	}
	case RibbonButton::PASTE:
	{
		if (RibbonButton::ButtonType::MOVE == lastOperation && operationFrom != "")
		{
			if (fs.move(operationFrom, folderView->getCurrentPath()))
			{
				folderView->loadDirectory(paths[currentPathIndex], true);
				//success
			}
		}
		else if (RibbonButton::ButtonType::COPY == lastOperation &&  operationFrom != "")
		{
			if (fs.copy(operationFrom, folderView->getCurrentPath()))
			{
				folderView->loadDirectory(paths[currentPathIndex], true);
				//success
			}
		}
		lastOperation = RibbonButton::INVALID;
		break;
	}
	case RibbonButton::BACK:
	{
		if (currentPathIndex > 0)
		{
			currentPathIndex--;
			folderView->loadDirectory(paths[currentPathIndex], false);
			refresh();
		}
		break;
	}
	case RibbonButton::FORTH:
	{
		if (currentPathIndex < paths.size() - 1)
		{
			currentPathIndex++;
			folderView->loadDirectory(paths[currentPathIndex], false);
			refresh();
		}
		break;
	}
	default:
		break;
	}
}

void Ribbon::processInput()
{
	if (lastOperation == RibbonButton::INVALID)
	{
		folderView->loadDirectory(directoryTextBox.getInputString(), true);
		directoryTextBox.setInputString("");
	}
	else if (lastOperation == RibbonButton::RENAME)
	{
		if (directoryTextBox.getInputString() != "" && folderView->getSelectedFilePath() != ""&& fs.rename(folderView->getSelectedFilePath(), directoryTextBox.getInputString()))
		{
			cout << "Rename Successful!\n";
		}
		else
		{
			cout << "OperationFailed!\n";
		}
		directoryTextBox.setInputString("");
		folderView->refreshDirectory();
		lastOperation = RibbonButton::INVALID;
	}
	else if (lastOperation == RibbonButton::NEW_FILE)
	{
		if (directoryTextBox.getInputString()!="" && fs.newFile(folderView->getCurrentPath(), directoryTextBox.getInputString()))
		{
		cout << "File Created!\n";
		}
		else
		{
		cout << "OperationFailed!\n";
		}
		directoryTextBox.setInputString("");
		folderView->refreshDirectory();
		lastOperation = RibbonButton::INVALID;
	}
	else if (lastOperation == RibbonButton::NEW_FOLDER)
	{
		if (directoryTextBox.getInputString()!=""&& fs.newFolder(folderView->getCurrentPath(), directoryTextBox.getInputString()))
		{
		cout << "Folder Created!\n";
		}
		else
		{
		cout << "OperationFailed!\n";
		}
		directoryTextBox.setInputString("");
		folderView->refreshDirectory();
		lastOperation = RibbonButton::INVALID;
	}
}
