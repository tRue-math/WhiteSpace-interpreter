#pragma once

#include"instruction.hpp"
#include"stack.hpp"
#include"heap.hpp"
#include<vector>
#include<map>
#include<string>

struct Program {
    std::vector<Instruction> instructions;
    std::map<std::string, int> labels;
};

struct State {
    Stack stack;
    Heap heap;
    Program program;
    int pc = 0;
    std::vector<int> call_stack;
};