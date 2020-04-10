#include <iostream>
#include <Windows.h>
#include <map>
#include <vector>
#include <functional>
#include <sstream>
#include <conio.h>
#include <iomanip>

#define PAUSE 200

using namespace std;
using Handler = function<void(void)>;

struct Request {
	LPCRITICAL_SECTION csection;
	int number;
};

vector<HANDLE> container;
map<unsigned long, int> shouldClose;

int cursor = -1;

void menu() {
	system("cls");
	cout << "Thread Sync" << endl;
	cout << "\t'+' to add new thread" << endl;
	cout << "\t'-' to close thread" << endl;
	cout << "\t'q' to close program" << endl << endl;
}

DWORD WINAPI mainWorker(void* buffer) {
	Request *request = (Request *)buffer;

	while (true) {
		auto shouldCloseIter = shouldClose.find(GetCurrentThreadId());
		if (shouldCloseIter == shouldClose.end()) {
			EnterCriticalSection(request->csection);
			if (request->number != cursor + 1) {
				LeaveCriticalSection(request->csection);
				continue;
			}
			if (request->number == container.size() - 1) {
				cursor = -1;
			} else {
				cursor = request->number;
			}

			if (request->number == 0) {
				Sleep(200);
				menu();
			}
			stringstream sstream;
			sstream << "Message from thread id #" << GetCurrentThreadId() << ":" << request->number << endl;
			for (auto symbol : sstream.str()) {
				cout << symbol;
			}
			LeaveCriticalSection(request->csection);
		}
	}
	return 1;
}

int main() {
	CRITICAL_SECTION csection;
	InitializeCriticalSectionAndSpinCount(&csection, 1000);

	map<char, Handler> library = {
		{ '+', [&]() {
			int size = container.size();
			Request *req = new Request{ &csection, size };
			HANDLE thread = CreateThread(NULL, BUFSIZ, mainWorker, (void *)req, NULL, NULL);
			container.push_back(thread);
		}},
		{'-', [&]() {
			if (container.size() == 0) {
				menu();
				return;
			}
			HANDLE tHandler = container.back();
			container.pop_back();
			cursor = -1;
			shouldClose.insert({ GetThreadId(tHandler), 0 });
			CloseHandle(tHandler);
			
			if (container.size() == 0) menu();
		}},
		{'q', [&]() {
			DeleteCriticalSection(&csection);
			exit(0);
		}}
	};

	menu();
	while (true) {
		char choice = _getch();
		if (library.find(choice) != library.end()) {
			Handler handler = library[choice];
			handler();
		}
	}

	return 0;
}