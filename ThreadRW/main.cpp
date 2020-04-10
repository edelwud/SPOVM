#include <iostream>
#include <dlfcn.h>
#include <mutex>
#include <condition_variable>
#include <thread>
#include <sstream>
#include <string.h>

using namespace std;

typedef bool(*ReadFileAsync_t)(char[BUFSIZ], char[BUFSIZ]);
typedef bool(*WriteFileAsync_t)(char[BUFSIZ], char[BUFSIZ]);


mutex locker;
condition_variable readed;
condition_variable writed;

int currentFile = 0;
int filesNum = 3;
char buffer[BUFSIZ]{ 0 };

bool ReadFileAsync(void *handle, string file, char dest[BUFSIZ]) {
    ReadFileAsync_t reader = (ReadFileAsync_t)dlsym(handle, "ReadFileAsync");
    const char* error = dlerror();
    if (error) {
        return false;
    }

	stringstream stream;
	stream << "./files" << "/" << file << ".txt";
	if (reader((char *)stream.str().data(), dest) == false) {
		return false;
	}
	return true;
}

bool WriteFileAsync(void *handle, string file, char dest[BUFSIZ]) {
    WriteFileAsync_t writer = (WriteFileAsync_t)dlsym(handle, "WriteFileAsync");
    const char* error = dlerror();
    if (error) {
        return false;
    }

	stringstream stream;
	stream << "files" << "/" << file << ".txt";
	if (writer((char *)stream.str().data(), dest) == false) {
		return false;
	}
	return true;
}

bool AppendFileAsync(void *handle, string file, char dest[BUFSIZ]) {
	char source[BUFSIZ]{ 0 };
	if (ReadFileAsync(handle, file, source) == false) {
		return false;
	}
	string result = source;
	result += dest;

	char resultBuffer[BUFSIZ]{ 0 };
	memcpy(resultBuffer, result.data(), result.size());

	if (WriteFileAsync(handle, file, resultBuffer) == false) {
		return false;
	}
	return true;
}

int main() {

    void* handle = dlopen("./lib/libAsync.so", RTLD_LAZY);
    if (!handle) {
        cout << "Error" << dlerror() << endl;
        return 0;
    }

    WriteFileAsync(handle, "result", buffer);

	thread readerThread([&]() {
		while (true) {
			unique_lock<mutex> uniqueLocker(locker);
			writed.wait(uniqueLocker);
            cout << "OK" << endl;
			if (ReadFileAsync(handle, to_string(++currentFile), buffer) == false) {
				continue;
			}

			thread tr([]() {
		        this_thread::sleep_for(chrono::milliseconds(100));
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

			if (AppendFileAsync(handle, "result", (char*)buffer) == false) {
				break;
			}
			memset(buffer, 0, BUFSIZ);

			thread tr([]() {
		        this_thread::sleep_for(chrono::milliseconds(100));
				writed.notify_one();
			});
			tr.detach();
			
			if (currentFile == filesNum) {
				break;
			}
		}
	});

	thread init([]() {
		this_thread::sleep_for(chrono::milliseconds(100));
		writed.notify_one(); 
	});

	readerThread.join();
	writerThread.join();
	init.join();

    dlclose(handle);
    return 0;
} 
