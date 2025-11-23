#pragma once

#include<vector>
#include<stdexcept>
#include<iostream>

class Stack {
public:
    int top() const;
    int pop();
    void push(int value);
    void dup();
    void swap();
    void drop();
    void copy(int index);
    void delete_n(int num);

    void debug_print() const;
private:
    std::vector<int> data;
};