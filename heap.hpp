#pragma once

#include<map>

class Heap {
public:
    void store(int address, int value);
    int load(int address);
private:
    std::map<int, int> storage;
}