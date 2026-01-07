#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "code_gen.h"
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
    std::cout << "NAME:" << std::endl;
    std::cout << node->name.lexeme << std::endl;
    std::cout << std::endl << "PARAMS:" << std::endl;
    for (auto& param : node->parameters) {
        std::cout << param.lexeme << std::endl;
    }
    std::cout << std::endl << "BODY:" << std::endl;
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
    // std::vector<std::unique_ptr<FunctionNode>> func_node_ptrs = parser.parseProgram();
    // for (const auto& func_node_ptr : func_node_ptrs) {
    //     printFunction(func_node_ptr.get());
    //     std::cout << "============" << std::endl;
    // }

    // std::unique_ptr<Node> node_ptr = parser.parseBraces();
    std::unique_ptr<FunctionNode> node_ptr = parser.parseFunction();
    CodeGen cg;
    // cg.EvaluateStatements(node_ptr.get());
    cg.EvaluateFunction(node_ptr.get());
    std::cout << cg.GetCode();
    return 0;
}
