#include "AHFileExplorer.h"

AHFileExplorer::AHFileExplorer()
	:
	folderView(Vector2f(350, 600)),
	ribbon(&folderView, Vector2f(600, 100))
{
	folderView.attachRibbon(&ribbon);
	folderView.setBackgroundColor(Color::White);

	folderView.setPosition(Vector2f(0, 0));
	ribbon.setPosition(Vector2f(0, 0));

	folderIcon.loadFromFile("resources/images/smallFolder.png");
	thisPcIcon.loadFromFile("resources/images/thisPc.png");
	driveIcon.loadFromFile("resources/images/smallDrive.png");

	font.loadFromFile("resources/fonts/segoeuil.ttf");

}

AHFileExplorer::~AHFileExplorer()
{
}

void AHFileExplorer::run()
{
	Vector2f winSize = { 600,600 };
	sf::RenderWindow window(sf::VideoMode(winSize.x, winSize.y), "AHFileExplorer");

	View mainView(FloatRect({ 0, 0 }, winSize));
	mainView.setViewport(FloatRect(0, 0, 1, 1));

	TreeNode treeView("This PC", "", thisPcIcon, font);
	loadDrives(&treeView, driveIcon, font);
	treeView.expandNode(true);


	Container leftPanel({ 0,100,250,winSize.y - 100 }, treeView, winSize);
	Container rightPanel({ 250,100,winSize.x - 250,winSize.y - 100 }, folderView, winSize);

	Control * oldEvent = nullptr;

	while (window.isOpen())
	{
		sf::Event event;

		while (window.pollEvent(event))
		{
			if (event.type == event.Closed)
			{
				window.close();
			}
			else if (event.type == sf::Event::Resized)
			{
				winSize = { float(event.size.width) , float(event.size.height) };
				leftPanel.resize({ 0,100,250,winSize.y - 100 }, winSize);
				rightPanel.resize({ 250,100,winSize.x - 250,winSize.y - 100 }, winSize);

				ribbon.setSize(Vector2f(window.getSize().x, 100));
				ribbon.positionButtons();

				folderView.setSize(Vector2f(window.getSize().x - 250, window.getSize().y - 100));
				folderView.positionFiles();
				folderView.positionDrives();

				mainView.reset(FloatRect({ 0, 0 }, winSize));
			}
			else if (event.type == Event::MouseButtonPressed)
			{
				if (oldEvent)
				{
					oldEvent->setBackgroundColor(Color::Transparent);
					oldEvent = nullptr;
				}
				Vector2i mousePos = sf::Mouse::getPosition(window);
				if (leftPanel.contains({ (float)mousePos.x,(float)mousePos.y }))
				{
					TreeNode * ptr = (TreeNode*)leftPanel.handleEvent(window, event, mousePos);
					if (ptr)
					{
						oldEvent = ptr;
						if (!ptr->getNoOfChilds())
						{
							populateNode(ptr, folderIcon, font);
						}
						ptr->expandNode(!ptr->isNodeExpanded());
						ptr->setBackgroundColor(Color(51, 190, 198, 150));

						leftPanel.updateControlSize({ treeView.getGlobalBounds().width, treeView.getGlobalBounds().height });

						string address = "\\";
						for (TreeNode * tn = ptr; tn != nullptr; tn = tn->getParent())
						{
							address = "\\" + tn->getName() + address;
						}
						address.replace(0, 2, "");

						folderView.loadDirectory(address);

						rightPanel.updateControlSize({ folderView.getGlobalBounds().width,folderView.getGlobalBounds().height });
					}
				}
			}
			Vector2i mousePos = Mouse::getPosition(window);
			if (leftPanel.contains({ (float)mousePos.x,(float)mousePos.y }))
			{
				leftPanel.handleEvent(window, event, mousePos);
			}
			else if (rightPanel.contains({ (float)mousePos.x,(float)mousePos.y }))
			{
				rightPanel.handleEvent(window, event, mousePos);
				rightPanel.updateControlSize({ folderView.getGlobalBounds().width,folderView.getGlobalBounds().height });
			}
			else /*if (ribbon.contains({ (float)mousePos.x,(float)mousePos.y }))*/
			{
				ribbon.handleEvent(event, { (float)mousePos.x,(float)mousePos.y });
			}
		}

		window.clear(sf::Color(250, 250, 250, 1));
		window.draw(leftPanel);
		window.draw(rightPanel);
		window.setView(mainView);
		window.draw(ribbon);
		window.display();
	}

}

void AHFileExplorer::populateNode(TreeNode * ptr, Texture & ft, Font & f)
{
	string address = "\\";
	for (TreeNode * tn = ptr; tn != nullptr; tn = tn->getParent())
	{
		address = "\\" + tn->getName() + address;
	}
	address.replace(0, 2, "");
	vector<string> list = FileSystem::getDirectoriesList(address);

	for (auto& str : list)
	{
		ptr->addNode(TreeNode(str, str, ft, f));
	}
}

void AHFileExplorer::loadDrives(TreeNode * root, Texture & ft, Font & f)
{
	vector<string> list = FileSystem::getDrivesList();
	for (auto & str : list)
	{
		str.pop_back();
		root->addNode(TreeNode(str, str, ft, f));
	}
}

