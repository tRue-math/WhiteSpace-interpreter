#pragma once

#include<variant>
#include<string>

// Stack

struct Push {
    int value;
};

struct Dup {};

struct Swap {};

struct Drop {};

struct Copy {
    int index;
};

struct Delete {
    int num;
};

// Arithmetic

struct Add {};

struct Sub {};

struct Mul {};

struct Div {};

struct Mod {};

// Heap

struct Store {};

struct Load {};

// I/O

struct PrintASCII {};

struct PrintInt {};

struct ReadASCII {};

struct ReadInt {};

// Control Flow

struct Label {
    std::string label;
};

struct Jump {
    std::string label;
};

struct JumpZero {
    std::string label;
};

struct JumpNeg {
    std::string label;
};

struct Call {
    std::string label;
};

struct Return {};

struct Halt {};

using ImpStack = std::variant<
    Push,
    Dup,
    Swap,
    Drop,
    Copy,
    Delete
>;
using ImpArithmetic = std::variant<
    Add,
    Sub,
    Mul,
    Div,
    Mod
>;
using ImpHeap = std::variant<
    Store,
    Load
>;
using ImpIO = std::variant<
    PrintASCII,
    PrintInt,
    ReadASCII,
    ReadInt
>;
using ImpControlFlow = std::variant<
    Label,
    Jump,
    JumpZero,
    JumpNeg,
    Call,
    Return,
    Halt
>;

using Instruction = std::variant<
    ImpStack,
    ImpArithmetic,
    ImpHeap,
    ImpIO,
    ImpControlFlow
>;