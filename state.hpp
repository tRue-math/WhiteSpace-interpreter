#include"instruction.hpp"
#include"stack.hpp"
#include"heap.hpp"

using Program = std::vector<Instruction>;

struct State {
    Stack stack;
    Heap heap;
    Program program;
    int pc = 0;
    std::vector<int> call_stack;
};