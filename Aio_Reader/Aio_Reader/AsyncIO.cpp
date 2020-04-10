#include "pch.h"
#include "AsyncIO.h"

void Converter(char source[BUFSIZ], wchar_t dest[BUFSIZ]) {
	MultiByteToWideChar(0, 0, source, strlen(source) + 1, dest, strlen(source) + 1);
}

BOOL ReadFileAsync(char path[BUFSIZ], char buffer[BUFSIZ]) {
	wchar_t dest[BUFSIZ];
	Converter(path, dest);
	HANDLE fileHandler = CreateFile(dest, GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (fileHandler == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	OVERLAPPED ol{ 0 };
	if (ReadFileEx(fileHandler, buffer, BUFSIZ - 1, &ol, [](DWORD, DWORD, LPOVERLAPPED) {}) == FALSE) {
		CloseHandle(fileHandler);
		return FALSE;
	}
	SleepEx(5000, TRUE);
	CloseHandle(fileHandler);
	return TRUE;
}

BOOL WriteFileAsync(char path[BUFSIZ], char buffer[BUFSIZ]) {
	wchar_t dest[BUFSIZ];
	Converter(path, dest);
	HANDLE fileHandler = CreateFile(dest, GENERIC_ALL, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (fileHandler == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	SetEndOfFile(fileHandler);
	OVERLAPPED ol{ 0 };
	if (WriteFileEx(fileHandler, buffer, strlen(buffer), &ol, [](DWORD, DWORD, LPOVERLAPPED) {}) == FALSE) {
		CloseHandle(fileHandler);
		return FALSE;
	}
	SleepEx(5000, TRUE);
	CloseHandle(fileHandler);
	return TRUE;
}

