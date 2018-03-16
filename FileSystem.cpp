#include "FileSystem.h"
#include <Windows.h>
bool FileSystem::remove(const string path)
{
	if (path[path.length() - 1] != '\\')
	{
		return DeleteFile(path.c_str());
	}
	string dir = path;
	dir.pop_back();
	dir.push_back('\0');
	dir.push_back('\0');

	SHFILEOPSTRUCT file_op = {
		NULL,
		FO_DELETE,
		dir.c_str(),
		"",
		FOF_NOCONFIRMATION |
		FOF_NOERRORUI |
		FOF_SILENT,
		false,
		0,
		"" };
	return !SHFileOperation(&file_op);
}

bool FileSystem::rename(const string oldPath, const string newName)
{
	string newPath = oldPath.substr(0, oldPath.find_last_of('\\', oldPath.length() - 2) + 1) + newName;
	if (oldPath[oldPath.length() - 1] == '\\')
	{
		newPath += '\\';
	}
	else
	{
		newPath += oldPath.substr(oldPath.find_last_of('.'));//append extesion
	}
	return	MoveFile(oldPath.c_str(), newPath.c_str());
}

bool FileSystem::move(const string source, const string destination)
{
	if (source[source.length() - 1] != '\\')
	{
		return	MoveFile(source.c_str(), (destination + source.substr(source.find_last_of('\\', source.length() - 2))).c_str());
	}
	string dir = source;
	dir.pop_back();

	SHFILEOPSTRUCT file_op = {
		NULL,
		FO_MOVE,
		dir.c_str(),
		destination.c_str(),
		FOF_NOCONFIRMATION |
		FOF_NOERRORUI |
		FOF_SILENT,
		false,
		0,
		"" };
	return !SHFileOperation(&file_op);
}

bool FileSystem::copy(const string source, const string destination)
{
	if (source[source.length() - 1] != '\\')
	{
		string desti = destination + source.substr(source.find_last_of('\\') + 1);
		return CopyFile(source.c_str(), desti.c_str(), true);
	}
	string dir = source;
	dir.pop_back();

	SHFILEOPSTRUCT file_op = {
		NULL,
		FO_COPY,
		dir.c_str(),
		destination.c_str(),
		FOF_NOCONFIRMATION |
		FOF_NOERRORUI |
		FOF_SILENT,
		false,
		0,
		"" };
	return !SHFileOperation(&file_op);
}

bool FileSystem::newFile(const string path, const string name)
{
	HANDLE newFile = CreateFileA(
		(path + name).c_str(),
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_DELETE | FILE_SHARE_READ | FILE_SHARE_WRITE,
		nullptr,
		CREATE_NEW,
		FILE_ATTRIBUTE_NORMAL
		, nullptr);
	if(newFile==INVALID_HANDLE_VALUE)
	{
		return false;
	}
	CloseHandle(newFile);
	return true;
}

bool FileSystem::newFolder(const string path, const string name)
{
	return CreateDirectory((path + name + '\\').c_str(), NULL);
}

vector<string> FileSystem::getDirectoriesList(const string & path)
{
	vector<string> filesList;

	unsigned long attribute = GetFileAttributes(path.c_str());
	if (attribute & FILE_ATTRIBUTE_DIRECTORY)
	{
		WIN32_FIND_DATA findFileData;
		void * findFileHandel = FindFirstFile((path + '*').c_str(), &findFileData);
		do
		{
			if ((findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN) == 0 && findFileData.cFileName[0] != '.')
			{

				bool isDirectory = bool(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
				if (isDirectory)
				{
					filesList.push_back(findFileData.cFileName);
				}

			}

		} while (FindNextFile(findFileHandel, &findFileData));
		FindClose(findFileHandel);
	}
	return filesList;
}

vector<string> FileSystem::getDrivesList()
{
	vector<string> filesList;

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
			filesList.push_back(tempBuf);
			tempPos = 0;
		}
		else
		{
			++tempPos;
		}
		++pos;
	}

	return filesList;
}
