#include "heap.hpp"

class Heap {
    std::map<int, int> storage;
public:
    void store(int address, int value) {
        storage[address] = value;
    }

    int load(int address) {
        return storage[address];
    }
};