#pragma once

#include<map>
#include<iostream>

class Heap {
public:
    void store(int address, int value);
    int load(int address);

    void debug_print() const;
private:
    std::map<int, int> storage;
};