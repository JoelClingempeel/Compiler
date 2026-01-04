#include <fstream>
#include <iostream>
#include <memory>
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

void printTree(Node* node, int depth) {
    if (!node) {
        return;
    }

    for (int i = 0; i < depth; i++) {
        std::cout << "   ";
    }
    std::cout << node->token.lexeme << std::endl;
    for (const auto& child : node->children) {
        printTree(child.get(), depth + 1);
    }
}

void printFunction(FunctionNode* node) {
    std::cout << node->name.lexeme << std::endl;
    for (auto& param : node->parameters) {
        std::cout << param.lexeme << std::endl;
    }
    printTree(node->body.get(), 0);
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

    Parser parser(lexer.tokens);
    std::unique_ptr<FunctionNode> node_ptr = parser.parseFunction();
    printFunction(node_ptr.get());

    return 0;
}
