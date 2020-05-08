/*
    Дефрагментация

    класс Memory (alloc, defrag, status)
    класс для работы с блоком памяти MemBlock -> MemElement

*/
#include <iostream>
#include <random>
#include <ctime>
#include <iomanip>
#include <conio.h>

#include "Memory.h"

using namespace std;

int main()
{
    srand(time(0));
    Memory::Init();

    while (true) {
        cout << "1. Allocate memory\n2. Defragmentation\n3. Show status" << endl;
        char choice = _getch();

        switch (choice)
        {
        case '1': {
            int size = rand() % 1024;
            cout << "Do u want to alloc " << size << " bytes (Y, n)" << endl;
            choice = _getch();

            if (choice != 'n') {
                int offset;
                try {
                    offset = (int)Memory::Alloc(size);
                }
                catch (exception e) {
                    cout << e.what() << endl;
                    break;
                }
                cout << "May be free (Y, n)?" << endl;
                choice = _getch();
                if (choice != 'n') {
                    offset = offset - (int)Memory::amount;
                    Memory::Free({ offset, offset + size });
                    _getch();
                }
            }
        }
                break;
        case '2':
            Memory::Defrag();
            break;
        case '3':
            cout << setw(10) << "Size" << setw(10) << "Start" << setw(10) << "End" << endl;
            for (auto block : Memory::allocated) {
                auto [start, end] = block;
                cout << setw(10) << end - start << setw(10) << start << setw(10) << end << endl;
            }
            _getch();
            break;
        default:
            break;
        }
        system("cls");
    }

    return 0;
}