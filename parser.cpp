#include"parser.hpp"

std::string make_whitespace_visible(std::string& input) {
    for (int i = 0; i < input.size(); ++i) {
        if (input[i] == '\r' && input[i + 1] == '\n') {
            input[i] = 'a'; // dummy character
        }
    }
    std::string output;
    for (char c : input) {
        switch (c) {
            case ' ':
                output += "S";
                break;
            case '\t':
                output += "T";
                break;
            case '\n':
                output += "L";
                break;
        }
    }
    return output;
}

int parse_number(const std::string& code, int& pos) {
    bool negative = false;
    if (code[pos] == 'T') {
        negative = true;
    }
    pos++;
    int value = 0;
    while (code[pos] != 'L') {
        value <<= 1;
        if (code[pos] == 'T') {
            value |= 1;
        }
        pos++;
    }
    pos++; // skip L
    if (negative) {
        value = -value;
    }
    return value;
}

std::string parse_label(const std::string& code, int& pos) {
    std::string label = "";
    while (code[pos] != 'L') {
        label += code[pos];
        pos++;
    }
    pos++; // skip L
    return label;
}

int parse_instruction(const std::string& code, int pos, Instruction& instr) {
    int start_pos = pos;
    if(code[pos] == 'S') {
        pos++;
        if (code[pos] == 'S') {
            // Push
            pos++;
            int value = parse_number(code, pos);
            instr = Push{value};
            return pos;
        }
        if (code[pos] == 'L') {
            pos++;
            if (code[pos] == 'S') {
                // Dup
                pos++;
                instr = Dup{};
                return pos;
            } else if (code[pos] == 'T') {
                // Swap
                pos++;
                instr = Swap{};
                return pos;
            } else if (code[pos] == 'L') {
                // Drop
                pos++;
                instr = Drop{};
                return pos;
            }
        }
        if (code[pos] == 'T') {
            pos++;
            if (code[pos] == 'S') {
                // Copy
                pos++;
                int index = parse_number(code, pos);
                instr = Copy{index};
                return pos;
            } else if (code[pos] == 'L') {
                // Delete
                pos++;
                int value = parse_number(code, pos);
                instr = Delete{value};
                return pos;
            }
        }
    }
    if (code[pos] == 'T') {
        pos++;
        if (code[pos] == 'S') {
            // Arithmetic
            pos++;
            if (code[pos] == 'S') {
                pos++;
                if (code[pos] == 'S') {
                    // Add
                    pos++;
                    instr = Add{};
                    return pos;
                } else if (code[pos] == 'T') {
                    // Sub
                    pos++;
                    instr = Sub{};
                    return pos;
                } else if (code[pos] == 'L') {
                    // Mul
                    pos++;
                    instr = Mul{};
                    return pos;
                }
            }
            if (code[pos] == 'T') {
                pos++;
                if (code[pos] == 'S') {
                    // Div
                    pos++;
                    instr = Div{};
                    return pos;
                } else if (code[pos] == 'T') {
                    // Mod
                    pos++;
                    instr = Mod{};
                    return pos;
                }
            }
        }
        if (code[pos] == 'T') {
            // Heap
            pos++;
            if (code[pos] == 'S') {
                // Store
                pos++;
                instr = Store{};
                return pos;
            } else if (code[pos] == 'T') {
                // Load
                pos++;
                instr = Load{};
                return pos;
            }
        }
        if (code[pos] == 'L') {
            // I/O
            pos++;
            if (code[pos] == 'S') {
                pos++;
                if (code[pos] == 'S') {
                    // Print ASCII
                    pos++;
                    instr = PrintASCII{};
                    return pos;
                } else if (code[pos] == 'T') {
                    // Print Int
                    pos++;
                    instr = PrintInt{};
                    return pos;
                }
            }
            if (code[pos] == 'T') {
                pos++;
                if (code[pos] == 'S') {
                    // Read ASCII
                    pos++;
                    instr = ReadASCII{};
                    return pos;
                } else if (code[pos] == 'T') {
                    // Read Int
                    pos++;
                    instr = ReadInt{};
                    return pos;
                }
            }
        }
    }
    if (code[pos] == 'L') {
        // Control Flow
        pos++;
        if (code[pos] == 'S') {
            pos++;
            if (code[pos] == 'S') {
                // Label
                pos++;
                std::string label = parse_label(code, pos);
                instr = Label{label};
                return pos;
            }
            if (code[pos] == 'T') {
                // Call
                pos++;
                std::string label = parse_label(code, pos);
                instr = Call{label};
                return pos;
            }
            if (code[pos] == 'L') {
                // Jump
                pos++;
                std::string label = parse_label(code, pos);
                instr = Jump{label};
                return pos;
            }
        }
        if (code[pos] == 'T') {
            pos++;
            if (code[pos] == 'S') {
                // JumpZero
                pos++;
                std::string label = parse_label(code, pos);
                instr = JumpZero{label};
                return pos;
            }
            if (code[pos] == 'T') {
                // JumpNeg
                pos++;
                std::string label = parse_label(code, pos);
                instr = JumpNeg{label};
                return pos;
            }
            if (code[pos] == 'L') {
                // Return
                pos++;
                instr = Return{};
                return pos;
            }
        }
    }
    return pos;
}

void label_resolution(State& state) {
    for (int i = 0; i < state.program.instructions.size(); ++i) {
        const Instruction& instr = state.program.instructions[i];
        if (std::holds_alternative<ImpControlFlow>(instr)) {
            const ImpControlFlow& cf_instr = std::get<ImpControlFlow>(instr);
            if (std::holds_alternative<Label>(cf_instr)) {
                const Label& label_instr = std::get<Label>(cf_instr);
                state.program.labels[label_instr.label] = i;
            }
        }
    }
}

State parse_string_to_state(std::string& code) {
    State state;
    std::string visible_code = make_whitespace_visible(code);
    // std::cout<<visible_code<<std::endl;
    int pos=0;
    while (pos < visible_code.size()) {
        Instruction instr;
        int new_pos = parse_instruction(visible_code, pos, instr);
        if (new_pos == pos) {
            throw std::runtime_error("Failed to parse instruction at position " + std::to_string(pos) + ": " + visible_code.substr(pos, 10));
        }
        state.program.instructions.push_back(instr);
        // Debug output
        // {
        //     std::cout << "Parsed instruction at position " << pos << ": ";
        //     if (std::holds_alternative<ImpStack>(instr)) {
        //         std::cout << "ImpStack" << std::endl;
        //         if (std::holds_alternative<Push>(std::get<ImpStack>(instr))) {
        //             std::cout << "  Push " << std::get<Push>(std::get<ImpStack>(instr)).value << std::endl;
        //         }
        //     } else if (std::holds_alternative<ImpArithmetic>(instr)) {
        //         std::cout << "ImpArithmetic" << std::endl;
        //     } else if (std::holds_alternative<ImpHeap>(instr)) {
        //         std::cout << "ImpHeap" << std::endl;
        //     } else if (std::holds_alternative<ImpIO>(instr)) {
        //         std::cout << "ImpIO" << std::endl;
        //     } else if (std::holds_alternative<ImpControlFlow>(instr)) {
        //         std::cout << "ImpControlFlow" << std::endl;
        //         if (std::holds_alternative<Label>(std::get<ImpControlFlow>(instr))) {
        //             std::cout << "  Label " << std::get<Label>(std::get<ImpControlFlow>(instr)).label << std::endl;
        //         }
        //         if (std::holds_alternative<Jump>(std::get<ImpControlFlow>(instr))) {
        //             std::cout << "  Jump " << std::get<Jump>(std::get<ImpControlFlow>(instr)).label << std::endl;
        //         }
        //         if (std::holds_alternative<Call>(std::get<ImpControlFlow>(instr))) {
        //             std::cout << "  Call " << std::get<Call>(std::get<ImpControlFlow>(instr)).label << std::endl;
        //         }
        //         if (std::holds_alternative<JumpZero>(std::get<ImpControlFlow>(instr))) {
        //             std::cout << "  JumpZero " << std::get<JumpZero>(std::get<ImpControlFlow>(instr)).label << std::endl;
        //         }
        //         if (std::holds_alternative<JumpNeg>(std::get<ImpControlFlow>(instr))) {
        //             std::cout << "  JumpNeg " << std::get<JumpNeg>(std::get<ImpControlFlow>(instr)).label << std::endl;
        //         }
        //         if (std::holds_alternative<Return>(std::get<ImpControlFlow>(instr))) {
        //             std::cout << "  Return" << std::endl;
        //         }
        //         if (std::holds_alternative<Halt>(std::get<ImpControlFlow>(instr))) {
        //             std::cout << "  Halt" << std::endl;
        //         }
        //     }
        // }
        pos = new_pos;
    }
    label_resolution(state);
    return state;
}

