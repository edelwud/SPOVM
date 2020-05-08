#include "Memory.h"

void Memory::Init()
{
    exempted.emplace_back(0, MEMORY_SIZE);
}

void* Memory::Alloc(int size)
{
    bool emplaced = false;
    int offset = 0;

    for (auto it = exempted.begin(); it != exempted.end(); ++it) {
        auto& [start, end] = *it;
        if (size <= (end - start)) {
            offset = start;
            allocated.emplace_back(start, start + size);

            start += size;
            emplaced = true;

            if (start == end) exempted.erase(it);
            break;
        }
    }

    if (!emplaced) throw exception("No free memory, may me need defragmentation");

    return (void*)&amount[offset];
}

void Memory::Free(std::pair<int, int> element)
{
    auto toRemove = find(allocated.begin(), allocated.end(), element);
    allocated.erase(toRemove);
    exempted.push_back(element);
}

void Memory::Defrag()
{
    sort(allocated.begin(), allocated.end(), [](auto first, auto second) {
        return first.first > second.first;
        });

    int prev = 0;
    for (auto& obj : allocated) {
        auto &[start, end] = obj;
        int value = prev;
        int size = end - start;
        prev += end - start;
        start = value;
        end = value + size;
    }

    exempted.clear();
    exempted.emplace_back(prev, MEMORY_SIZE);
}
