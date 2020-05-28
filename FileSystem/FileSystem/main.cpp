#include <iostream>
#include <iomanip>
#include <map>
#include <functional>
#include <conio.h>

#include "FileSystem.h"


void ShowFileSystem(const FileSystem::Directory* root, int spaceSize) {
    cout << setw(spaceSize) << "" << root->folder << " DIR " << root->filesCount << " files" << endl;
    for (auto directory : root->directories) {
        ShowFileSystem(directory, spaceSize + 2);
    }
    for (auto file : root->files) {
        cout << setw(spaceSize + 2) << "" << file->filename << " FILE" << endl;
    }
}

using namespace std;

int main()
{
    FileSystem fsys;
    fsys.CreateStorage("storage");
    fsys.LoadStorage("storage");

    fsys.CreateFolder("main/example");
    fsys.CreateFile("main/example/example.exe", "simple info");
    fsys.CreateFile("main/example/example2.exe", "kek");
    fsys.CreateFile("main/example/example3.exe", "kek");
    fsys.CreateFile("main/example/example4.exe", "kek");
    fsys.CreateFile("main/example.exe", "lol");
    fsys.CreateFile("main/example.exe", "lol");
    fsys.CommitChanges();


    map<char, function<void(void)>> library = {
        {
            '1', [&]() {
                const FileSystem::Directory* root = fsys.GetRoot();
                ShowFileSystem(root, 0);
                _getch();
            }
        },
        {
            '2', [&]() {
                cout << "# Adding new file" << endl << endl;
                cout << "Enter filename: ";
                string filename;
                cin >> filename;

                cout << "Enter binary: ";
                string buffer;
                cin >> buffer;

                fsys.CreateFile(filename, buffer);
                fsys.CommitChanges();
            }
        },
        {
            '3', [&]() {
                cout << "# Opening file" << endl << endl;
                cout << "Enter filename: ";
                string filename;
                cin >> filename;

                try {
                    const FileSystem::File* file = fsys.OpenFile(filename);
                    cout << endl << file->buffer << endl;
                }
                catch (...) {
                    cout << "Cannot find file" << endl;
                }
                _getch();
            }
        },
        {
            '4', [&]() {
                cout << "# Editing file" << endl << endl;
                cout << "Enter filename: ";
                string filename;
                cin >> filename;

                cout << "Enter binary: ";
                string buffer;
                cin >> buffer;

                fsys.EditFile(filename, buffer);
                fsys.CommitChanges();
            }
        },
        {
            '5', [&]() {
                cout << "# Deleting file" << endl << endl;
                cout << "Enter filename: ";
                string filename;
                cin >> filename;

                fsys.DeleteFile(filename);
                fsys.CommitChanges();
            }
        },
        {
            '6', [&]() {
                cout << "# Creating directory" << endl << endl;
                cout << "Enter folder name: ";
                string folder;
                cin >> folder;

                fsys.CreateFolder(folder);
                fsys.CommitChanges();
            }
        },
        {
            '7', [&]() {
                cout << "# Deleting directory" << endl << endl;
                cout << "Enter folder name: ";
                string folder;
                cin >> folder;

                fsys.DeleteFolder(folder);
                fsys.CommitChanges();
            }
        },
        {
            '8', [&]() {
                exit(0);
            }
        }
    };

    while (true) {
        system("cls");
        cout << "# FileSystem" << endl << endl;
        cout << "1. Show files and directories" << endl;
        cout << "2. Create file" << endl;
        cout << "3. Open file" << endl;
        cout << "4. Edit file" << endl;
        cout << "5. Delete file" << endl;
        cout << "6. Create directory" << endl;
        cout << "7. Delete directory" << endl;
        cout << "8. Exit" << endl;

        char choice = _getch();
        system("cls");
        auto iter = library.find(choice);
        if (iter == library.end()) 
            continue;
        library[choice]();
    }

    return 0;
}