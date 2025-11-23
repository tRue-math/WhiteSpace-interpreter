#include "heap.hpp"

void Heap::store(int address, int value) {
    storage[address] = value;
}

int Heap::load(int address) {
    return storage[address];
}

void Heap::debug_print() const {
    for (const auto& pair : storage) {
        std::cout << "{" << pair.first << " : " << pair.second << "},";
    }
    std::cout << std::endl;
}