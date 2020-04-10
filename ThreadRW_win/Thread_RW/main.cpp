#include <iostream>
#include <thread>
#include <functional>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <sstream>
#include <Windows.h>

using namespace std;
typedef BOOL(__cdecl Reader)(char[BUFSIZ], char[BUFSIZ]);
typedef BOOL(__cdecl Writer)(char[BUFSIZ], char[BUFSIZ]);

mutex locker;
condition_variable readed;
condition_variable writed;

int currentFile = 0;
int filesNum = 3;
char buffer[BUFSIZ]{ 0 };

BOOL ReadFileAsync(HINSTANCE AsyncRW, string file, char dest[BUFSIZ]) {
	Reader* reader = (Reader*)GetProcAddress(AsyncRW, "ReadFileAsync");
	if (reader == nullptr) {
		return FALSE;
	}
	stringstream stream;
	stream << "includes" << "\\" << file << ".txt";
	if (reader((char *)stream.str().data(), dest) == FALSE) {
		return FALSE;
	}
	return TRUE;
}

BOOL WriteFileAsync(HINSTANCE AsyncRW, string file, char dest[BUFSIZ]) {
	Writer* writer = (Writer *)GetProcAddress(AsyncRW, "WriteFileAsync");
	if (writer == nullptr) {
		return FALSE;
	}
	stringstream stream;
	stream << "includes" << "\\" << file << ".txt";
	if (writer((char*)stream.str().data(), dest) == FALSE) {
		return FALSE;
	}
	return TRUE;
}

BOOL AppendFileAsync(HINSTANCE AsyncRW, string file, char dest[BUFSIZ]) {
	char source[BUFSIZ]{ 0 };
	if (ReadFileAsync(AsyncRW, file, source) == FALSE) {
		return FALSE;
	}
	string result = source;
	result += dest;

	char resultBuffer[BUFSIZ]{ 0 };
	memcpy_s(resultBuffer, BUFSIZ, result.data(), result.size());

	if (WriteFileAsync(AsyncRW, file, resultBuffer) == FALSE) {
		return FALSE;
	}
	return TRUE;
}

int main() {

	HINSTANCE AsyncRW = LoadLibrary("Aio_Reader.dll");
	if (AsyncRW == NULL) {
		cout << "Error: cannot load dynamic library" << endl;
		return 0;
	}

	WriteFileAsync(AsyncRW, "result", buffer);

	thread readerThread([&]() {
		while (true) {
			unique_lock<mutex> uniqueLocker(locker);
			writed.wait(uniqueLocker);

			if (ReadFileAsync(AsyncRW, to_string(++currentFile), buffer) == FALSE) {
				continue;
			}

			thread tr([]() {
				Sleep(100);
				readed.notify_one();
			});
			tr.detach();

			if (currentFile == filesNum) {
				break;
			}
		}
	});

	thread writerThread([=]() {
		while (true) {
			unique_lock<mutex> uniqueLocker(locker);
			readed.wait(uniqueLocker);

			if (AppendFileAsync(AsyncRW, "result", (char*)buffer) == FALSE) {
				break;
			}
			memset(buffer, 0, BUFSIZ);

			thread tr([]() {
				Sleep(100);
				writed.notify_one();
			});
			tr.detach();
			
			if (currentFile == filesNum) {
				break;
			}
		}
	});

	thread init([]() {
		Sleep(100);
		writed.notify_one(); 
	});

	readerThread.join();
	writerThread.join();
	init.join();
	FreeLibrary(AsyncRW);
	return 0;
}