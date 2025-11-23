#include"processor.hpp"
#include"parser.hpp"
#include"state.hpp"
#include<fstream>
#include<iostream>
#include<string>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <source_file>" << std::endl;
        return 1;
    }

    std::ifstream input_file(argv[1]);

    if (!input_file) {
        std::cerr << "Error: Could not open file " << argv[1] << std::endl;
        return 1;
    }

    std::string source_code((std::istreambuf_iterator<char>(input_file)),
                             std::istreambuf_iterator<char>());
    
    State state;
    try {
        state = parse_string_to_state(source_code);
    } catch (const std::runtime_error& e) {
        std::cerr << "Parsing error: " << e.what() << std::endl;
        return 1;
    }
    execute_program(state);
    return 0;
}