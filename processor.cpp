#include"processor.hpp"

struct Executor {
    State& state;

    Executor(State& s) : state(s) {}

    void operator()(const ImpStack& instr) const {
        std::visit(*this, instr);
    }
    void operator()(const ImpArithmetic& instr) const {
        std::visit(*this, instr);
    }
    void operator()(const ImpHeap& instr) const {
        std::visit(*this, instr);
    }
    void operator()(const ImpIO& instr) const {
        std::visit(*this, instr);
    }
    void operator()(const ImpControlFlow& instr) const {
        std::visit(*this, instr);
    }

    // Stack

    void operator()(const Push& instr) const {
        state.stack.push(instr.value);
        state.pc++;
    }
    void operator()(const Dup& instr) const {
        state.stack.dup();
        state.pc++;
    }
    void operator()(const Swap& instr) const {
        state.stack.swap();
        state.pc++;
    }
    void operator()(const Drop& instr) const {
        state.stack.drop();
        state.pc++;
    }
    void operator()(const Copy& instr) const {
        state.stack.copy(instr.index);
        state.pc++;
    }
    void operator()(const Delete& instr) const {
        state.stack.delete_n(instr.num);
        state.pc++;
    }

    // Arithmetic

    void operator()(const Add& instr) const {
        int b = state.stack.pop();
        int a = state.stack.pop();
        state.stack.push(a + b);
        state.pc++;
    }
    void operator()(const Sub& instr) const {
        int b = state.stack.pop();
        int a = state.stack.pop();
        state.stack.push(a - b);
        state.pc++;
    }
    void operator()(const Mul& instr) const {
        int b = state.stack.pop();
        int a = state.stack.pop();
        state.stack.push(a * b);
        state.pc++;
    }
    void operator()(const Div& instr) const {
        int b = state.stack.pop();
        int a = state.stack.pop();
        if (b == 0) {
            throw std::runtime_error("Division by zero");
        }
        state.stack.push(a / b);
        state.pc++;
    }
    void operator()(const Mod& instr) const {
        int b = state.stack.pop();
        int a = state.stack.pop();
        if (b == 0) {
            throw std::runtime_error("Modulo by zero");
        }
        state.stack.push(a % b);
        state.pc++;
    }

    // Heap

    void operator()(const Store& instr) const {
        int value = state.stack.pop();
        int address = state.stack.pop();
        state.heap.store(address, value);
        state.pc++;
    }
    void operator()(const Load& instr) const {
        int address = state.stack.pop();
        int value = state.heap.load(address);
        state.stack.push(value);
        state.pc++;
    }

    // I/O
    
    void operator()(const PrintASCII& instr) const {
        int value = state.stack.pop();
        std::cout << static_cast<char>(value);
        state.pc++;
    }
    void operator()(const PrintInt& instr) const {
        int value = state.stack.pop();
        std::cout << value;
        state.pc++;
    }
    void operator()(const ReadASCII& instr) const {
        char ch;
        std::cin.get(ch);
        if(ch == EOF) {
            state.stack.push(-1);
        } else {
            state.stack.push(static_cast<int>(ch));
        }
        state.pc++;
    }
    void operator()(const ReadInt& instr) const {
        std::string line;
        if (!std::getline(std::cin, line)) {
            state.stack.push(-1);
            state.pc++;
            return;
        }
        const std::regex number_regex("([-+]?\\d+)$");
        std::smatch match;
        if (!std::regex_search(line, match, number_regex)) {
            throw std::runtime_error("Invalid integer input");
        }

        try {
            int value = std::stoi(match.str(1));
            state.stack.push(value);
        } catch (const std::out_of_range&) {
            throw std::runtime_error("Integer input out of range");
        }
        state.pc++;
    }

    // Control Flow

    void operator()(const Label& instr) const {
        state.pc++;
    }
    void operator()(const Jump& instr) const {
        auto it = state.program.labels.find(instr.label);
        if (it == state.program.labels.end()) {
            throw std::runtime_error("Undefined label: " + std::to_string(instr.label));
        }
        state.pc = it->second;
    }
    void operator()(const JumpZero& instr) const {
        int value = state.stack.pop();
        if (value == 0) {
            auto it = state.program.labels.find(instr.label);
            if (it == state.program.labels.end()) {
                throw std::runtime_error("Undefined label: " + std::to_string(instr.label));
            }
            state.pc = it->second;
        } else {
            state.pc++;
        }
    }
    void operator()(const JumpNeg& instr) const {
        int value = state.stack.pop();
        if (value < 0) {
            auto it = state.program.labels.find(instr.label);
            if (it == state.program.labels.end()) {
                throw std::runtime_error("Undefined label: " + std::to_string(instr.label));
            }
            state.pc = it->second;
        } else {
            state.pc++;
        }
    }
    void operator()(const Call& instr) const {
        state.call_stack.push_back(state.pc + 1);
        auto it = state.program.labels.find(instr.label);
        if (it == state.program.labels.end()) {
            throw std::runtime_error("Undefined label: " + std::to_string(instr.label));
        }
        state.pc = it->second;
    }
    void operator()(const Return& instr) const {
        if (state.call_stack.empty()) {
            throw std::runtime_error("Call stack underflow on return");
        }
        state.pc = state.call_stack.back();
        state.call_stack.pop_back();
    }
    void operator()(const Halt& instr) const {
        state.pc = state.program.instructions.size();
    }
};


void execute_instruction(const Instruction& instr, State& state) {
    Executor executor{state};
    std::visit(executor, instr);
}
void execute_program(State& state) {
    while (0<=state.pc && state.pc < state.program.instructions.size()) {
        const Instruction& instr = state.program.instructions[state.pc];
        execute_instruction(instr, state);
    }
    if (state.pc == state.program.instructions.size()) {
        return;
    }
    throw std::runtime_error("Program counter out of bounds");
}