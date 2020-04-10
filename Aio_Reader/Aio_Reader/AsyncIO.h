#pragma once

#define BUFSIZ 512

#ifdef ASYNCIO_EXPORTS
#define ASYNCIO_API __declspec(dllexport)
#else
#define ASYNCIO_API __declspec(dllimport)
#endif

using namespace std;

extern "C" ASYNCIO_API BOOL ReadFileAsync(char[BUFSIZ], char[BUFSIZ]);

extern "C" ASYNCIO_API BOOL WriteFileAsync(char[BUFSIZ], char[BUFSIZ]);