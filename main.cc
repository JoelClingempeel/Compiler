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
    printTree(node->left.get(), depth + 1);
    printTree(node->right.get(), depth + 1);
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
    std::unique_ptr<Node> node = parser.parseAssignment();
    printTree(node.get(), 0);

    return 0;
}
