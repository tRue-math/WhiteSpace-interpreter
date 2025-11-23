#include"state.hpp"
#include"heap.hpp"
#include"stack.hpp"
#include<variant>
#include<stdexcept>
#include<iostream>
#include<string>
#include<regex>

std::string make_whitespace_visible(const std::string& input);
State parse_string_to_state(std::string& code);