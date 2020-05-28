#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <functional>

#define MAIN_DIRECTORY_NAME "main"
#define DIRECTORY_SEPARATOR '/'
#define STORAGE_EXTENSION ".fs"

using namespace std;

class FileSystem
{
public:
	enum class Type {
		FILE,
		DIRECTORY
	};
	struct File {
		string filename = "";
		string buffer = "";
	};
	struct Directory {
		string folder = "";
		int filesCount = 0;
		vector<File*> files;
		vector<Directory*> directories;
	};
private:
	string fsysPath;
	string currentPath;
	Directory* directory;

	vector<string> SplitFileName(string filename);

	void ReadProcess(fstream& storage, Directory* directory, int filesCount);
	void WriteProcess(fstream& storage, Directory* directory);

	void FindAndEmplace(vector<string> path, Directory* directory, File* file);
	void FindAndEdit(vector<string> path, Directory* directory, string filename, string buffer);
	void FindAndErase(vector<string> path, Directory* directory, string filename);
public:
	void CreateStorage(string path);
	void LoadStorage(string path);

	const File* OpenFile(string path);
	void CreateFile(string filename, string buffer);
	void EditFile(string filename, string buffer);
	void DeleteFile(string filename);

	void CreateFolder(string folder);
	void DeleteFolder(string folder);

	void CommitChanges();

	const Directory* GetRoot();
};

