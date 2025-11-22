#include<vector>
#include<stdexcept>

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
private:
    std::vector<int> data;
}