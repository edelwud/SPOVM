#include "FileSystem.h"

vector<string> FileSystem::SplitFileName(string filename)
{
	vector<string> tokens;
	size_t prev = 0, pos = 0;
	do
	{
		pos = filename.find(DIRECTORY_SEPARATOR, prev);
		if (pos == string::npos) pos = filename.length();
		string token = filename.substr(prev, pos - prev);
		if (!token.empty()) tokens.push_back(token);
		prev = pos + 1;
	} while (pos < filename.length() && prev < filename.length());

	return tokens;
}

void FileSystem::ReadProcess(fstream& storage, Directory* directory, int filesCount)
{
	while (filesCount) {
		int elTypeComputed;
		int entityNameSize;
		char entityBuffer[BUFSIZ]{ 0 };

		storage.read((char*)&elTypeComputed, sizeof(int));
		storage.read((char*)&entityNameSize, sizeof(int));
		storage.read(entityBuffer, entityNameSize * sizeof(char));


		Type elType = (Type)elTypeComputed;
		if (elType == Type::FILE) {
			int bufferSize;
			char buffer[BUFSIZ]{ 0 };

			storage.read((char*)&bufferSize, sizeof(int));
			storage.read(buffer, bufferSize * sizeof(char));

			File* file = new File;
			file->filename = entityBuffer;
			file->buffer = buffer;

			directory->files.push_back(file);
			filesCount--;
		}

		if (elType == Type::DIRECTORY) {
			int filesCountCurrent;
			storage.read((char*)&filesCountCurrent, sizeof(int));

			Directory* subDirectory = new Directory;
			subDirectory->folder = entityBuffer;
			subDirectory->filesCount = filesCountCurrent;

			directory->directories.push_back(subDirectory);
			filesCount -= filesCountCurrent;
			this->ReadProcess(storage, subDirectory, filesCountCurrent);
		}
	}
}

void FileSystem::WriteProcess(fstream& storage, Directory* directory)
{
	int elTypeComputed = (int)Type::DIRECTORY;
	storage.write((char*)&elTypeComputed, sizeof(int));

	int folderNameSize = directory->folder.size();
	storage.write((char*)&folderNameSize, sizeof(int));
	storage << directory->folder.data();

	int filesCount = directory->filesCount;
	storage.write((char*)&filesCount, sizeof(int));

	for (auto dir : directory->directories) {
		this->WriteProcess(storage, dir);
	}

	for (auto file : directory->files) {
		int elTypeComputed = (int)Type::FILE;
		storage.write((char*)&elTypeComputed, sizeof(int));

		int filenameSize = file->filename.size();
		storage.write((char*)&filenameSize, sizeof(int));
		storage << file->filename.data();

		int bufferSize = file->buffer.size();
		storage.write((char*)&bufferSize, sizeof(int));
		storage << file->buffer.data();
	}
}

void FileSystem::FindAndEmplace(vector<string> path, Directory* directory, File* file)
{
	if (path[0] != directory->folder)
		throw runtime_error("Cannot find directory");

	directory->filesCount++;

	for (auto iter = ++path.begin(); iter != path.end(); ++iter) {
		bool changed = false;
		for (auto dir : directory->directories) {
			if (dir->folder == *iter) {
				changed = true;
				directory = dir;
				directory->filesCount++;
				break;
			}
		}
		if (!changed) {
			throw runtime_error("Cannot find directory");
		}
	}

	directory->files.push_back(file);
}

void FileSystem::FindAndEdit(vector<string> path, Directory* directory, string filename, string buffer)
{
	if (path[0] != directory->folder)
		throw runtime_error("Cannot find directory");

	for (auto iter = ++path.begin(); iter != path.end(); ++iter) {
		bool changed = false;
		for (auto dir : directory->directories) {
			if (dir->folder == *iter) {
				changed = true;
				directory = dir;
				break;
			}
		}
		if (!changed) {
			throw runtime_error("Cannot find directory");
		}
	}
	auto iter = find_if(directory->files.begin(), directory->files.end(), [&](File* file) {
		return file->filename == filename;
	});
	(*iter)->buffer = buffer;
}

void FileSystem::FindAndErase(vector<string> path, Directory* directory, string filename)
{
	if (path[0] != directory->folder)
		throw runtime_error("Cannot find directory");

	directory->filesCount--;

	for (auto iter = ++path.begin(); iter != path.end(); ++iter) {
		bool changed = false;
		for (auto dir : directory->directories) {
			if (dir->folder == *iter) {
				changed = true;
				directory = dir;
				directory->filesCount--;
				break;
			}
		}
		if (!changed) {
			throw runtime_error("Cannot find directory");
		}
	}
	auto iter = find_if(directory->files.begin(), directory->files.end(), [&](File* file) {
		return file->filename == filename;
	});
	delete* iter;
	directory->files.erase(iter);
}

void FileSystem::CreateStorage(string path)
{
	fstream storage(path + STORAGE_EXTENSION, ios::binary | fstream::in | fstream::out | fstream::trunc);
	this->fsysPath = path + STORAGE_EXTENSION;

	Type directory = Type::DIRECTORY;
	storage.write((char *)&directory, sizeof(int));

	string folder = MAIN_DIRECTORY_NAME;
	int folderNameSize = folder.size();
	storage.write((char*)&folderNameSize, sizeof(int));
	storage << folder.c_str();

	int files = 0;
	storage.write((char*)&files, sizeof(int));

	storage.close();
}

void FileSystem::LoadStorage(string path)
{
	fstream storage(path + STORAGE_EXTENSION, ios::binary | fstream::in | fstream::out);
	this->fsysPath = path + STORAGE_EXTENSION;

	int elTypeComputed;
	int filesCount;
	int folderNameSize;
	char folderBuffer[BUFSIZ]{ 0 };

	storage.read((char*)&elTypeComputed, sizeof(int));
	storage.read((char*)&folderNameSize, sizeof(int));
	storage.read(folderBuffer, folderNameSize*sizeof(char));

	storage.read((char*)&filesCount, sizeof(int));

	this->directory = new Directory;
	this->directory->folder = folderBuffer;
	this->directory->filesCount = filesCount;
	
	Type elType = (Type)elTypeComputed;
	if (elType != Type::DIRECTORY) {
		throw runtime_error("Damaged root directory");
	}

	if (this->directory->folder != MAIN_DIRECTORY_NAME) {
		throw runtime_error("Directory name is not specified");
	}

	this->ReadProcess(storage, this->directory, filesCount);
	
	storage.close();
}

const FileSystem::File* FileSystem::OpenFile(string filenameTemp)
{
	vector<string> elements = this->SplitFileName(filenameTemp);
	string filename = elements[elements.size() - 1];
	elements.erase(--elements.end(), elements.end());

	if (elements[0] != directory->folder)
		throw runtime_error("Cannot find directory");

	for (auto iter = ++elements.begin(); iter != elements.end(); ++iter) {
		bool changed = false;
		for (auto dir : directory->directories) {
			if (dir->folder == *iter) {
				changed = true;
				directory = dir;
				break;
			}
		}
		if (!changed) {
			throw runtime_error("Cannot find directory");
		}
	}
	auto iter = find_if(directory->files.begin(), directory->files.end(), [&](File* file) {
		return file->filename == filename;
	});
	return *iter;
}

void FileSystem::CreateFile(string filenameTemp, string buffer)
{
	vector<string> elements = this->SplitFileName(filenameTemp);
	string filename = elements[elements.size() - 1];
	elements.erase(--elements.end(), elements.end());

	File* file = new File;
	file->filename = filename;
	file->buffer = buffer;

	this->FindAndEmplace(elements, this->directory, file);
}

void FileSystem::EditFile(string filenameTemp, string buffer)
{
	vector<string> elements = this->SplitFileName(filenameTemp);
	string filename = elements[elements.size() - 1];
	elements.erase(--elements.end(), elements.end());

	this->FindAndEdit(elements, this->directory, filename, buffer);
}

void FileSystem::DeleteFile(string filenameTemp)
{
	vector<string> elements = this->SplitFileName(filenameTemp);
	string filename = elements[elements.size() - 1];
	elements.erase(--elements.end(), elements.end());

	this->FindAndErase(elements, this->directory, filename);
}

void FileSystem::CreateFolder(string tempFolder)
{
	vector<string> path = this->SplitFileName(tempFolder);
	string folder = path[path.size() - 1];
	path.erase(--path.end(), path.end());

	Directory* directory = this->directory;
	if (path[0] != directory->folder)
		throw runtime_error("Cannot find path");

	for (auto iter = ++path.begin(); iter != path.end(); ++iter) {
		bool changed = false;
		for (auto dir : directory->directories) {
			if (dir->folder == *iter) {
				changed = true;
				directory = dir;
				break;
			}
		}
		if (!changed) {
			throw runtime_error("Cannot find directory");
		}
	}

	Directory* newDir = new Directory;
	newDir->folder = folder;
	directory->directories.push_back(newDir);
}

void FileSystem::DeleteFolder(string tempFolder)
{
	vector<string> path = this->SplitFileName(tempFolder);
	string folder = path[path.size() - 1];

	vector<Directory*> reduce;

	Directory* directory = this->directory;
	Directory* previos = directory;
	if (path[0] != directory->folder)
		throw runtime_error("Cannot find path");

	reduce.push_back(directory);

	for (auto iter = ++path.begin(); iter != path.end(); ++iter) {
		bool changed = false;
		for (auto dir : directory->directories) {
			if (dir->folder == *iter) {
				previos = directory;
				changed = true;
				directory = dir;
				reduce.push_back(directory);
				break;
			}
		}
		if (!changed) {
			throw runtime_error("Cannot find directory");
		}
	}

	int size = directory->filesCount;
	previos->directories.erase(find_if(previos->directories.begin(), previos->directories.end(), [=](Directory* dir) {
		return dir->folder == directory->folder;
	}));

	for (auto& dir : reduce) {
		dir->filesCount -= size;
	}
}

const FileSystem::Directory* FileSystem::GetRoot()
{
	return this->directory;
}


void FileSystem::CommitChanges()
{
	fstream storage(this->fsysPath, ios::binary | fstream::in | fstream::out);
	this->WriteProcess(storage, this->directory);
	storage.close();
}
