#include "stack.hpp"

int Stack::top() const {
    if (data.empty()) {
        throw std::runtime_error("Stack is empty, cannot get top element");
    }
    return data.back();
}

int Stack::pop() {
    if (data.empty()) {
        throw std::runtime_error("Stack is empty, cannot pop");
    }
    int value = data.back();
    data.pop_back();
    return value;
}

void Stack::push(int value) {
    data.push_back(value);
}

void Stack::dup() {
    if (data.empty()) {
        throw std::runtime_error("Stack is empty, cannot duplicate");
    }
    data.push_back(data.back());
}

void Stack::swap() {
    if (data.size() < 2) {
        throw std::runtime_error("Not enough elements to swap");
    }
    int a = data.back(); data.pop_back();
    int b = data.back(); data.pop_back(); 
    data.push_back(a);
    data.push_back(b);
}

void Stack::drop() {
    if (data.empty()) {
        throw std::runtime_error("Stack is empty, cannot drop");
    }
    data.pop_back();
}

void Stack::copy(int index) {
    if (index < 0 || index >= static_cast<int>(data.size())) {
        throw std::runtime_error("Index out of bounds for copy operation");
    }
    data.push_back(data[data.size() - 1 - index]);
}

void Stack::delete_n(int num) {
    if (num < 0 || num && num >= static_cast<int>(data.size())) {
        throw std::runtime_error("Not enough elements to delete");
    }
    int top=data.back();
    for (int i = 0; i < num + 1; ++i) {
        data.pop_back();
    }
    data.push_back(top);
}