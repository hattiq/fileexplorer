#pragma once
#include <string>
#include <vector>
using namespace std;
class FileSystem
{
public:
	static bool remove(const string path);
	static bool rename(const string oldPath, const string newName);
	static bool move(const string source, const string destination);
	static bool copy(const string source, const string destination);
	static bool newFile(const string path, const string name);
	static bool newFolder(const string path, const string name);
	static vector<string> getDirectoriesList(const string & path);
	static vector<string> getDrivesList();
};