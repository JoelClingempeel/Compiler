#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "lexer.h"
#include "parser.h"


std::string ReadFile(std::string path){
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << std::endl;
        exit(1);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string fullSource = buffer.str();
    return fullSource;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    std::string source = ReadFile(argv[1]);

    Lexer lexer(source);
    while (!lexer.EndReached()) {
        lexer.GetToken();
    }
    lexer.PrintTokens();

    return 0;
}
