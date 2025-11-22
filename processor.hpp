#include"instruction.hpp"
#include"state.hpp"
#include"heap.hpp"
#include"stack.hpp"
#include<variant>
#include<stdexcept>
#include<iostream>
#include<string>
#include<regex>

void execute_instruction(const Instruction& instr, State& state);
void execute_program(const Program& program, State& state);