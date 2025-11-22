#include "heap.hpp"

void Heap::store(int address, int value) {
    storage[address] = value;
}

int Heap::load(int address) {
    return storage[address];
}