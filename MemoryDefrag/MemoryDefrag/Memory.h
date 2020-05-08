#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

#define MEMORY_SIZE 4096

using namespace std;

class Memory
{
public:
    static inline vector<pair<int, int>> exempted;
    static inline vector<pair<int, int>> allocated;
    static inline char amount[MEMORY_SIZE]{ 0 };

    static void Init();

    static void* Alloc(int size);

    static void Free(pair<int, int> element);

    static void Defrag();
};
