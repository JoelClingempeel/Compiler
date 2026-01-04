#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <vector>

#include "lexer.h"
#include "parser.h"


struct Node {
    Token token;
    std::vector<std::unique_ptr<Node>> children;

    Node(Token token, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
        : token(token) {
        children.push_back(std::move(left));
        children.push_back(std::move(right));
    }

    Node(Token token, std::vector<std::unique_ptr<Node>> children)
        : token(token), children(std::move(children)) {}
};

struct FunctionNode {
    Token name;
    std::vector<Token> parameters;
    std::unique_ptr<Node> body;

    FunctionNode(Token name, std::vector<Token> parameters, std::unique_ptr<Node> body)
        : name(name), parameters(parameters), body(std::move(body)) {}
};

class Parser {
    int cur = 0;
    std::vector<Token> tokens;

    Token peek();
    Token previous();
    Token advance();
    bool endReached();
    bool match(TokenType type);
    void consume(TokenType type);

public:
    Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}
    std::unique_ptr<Node> parseFactor();
    std::unique_ptr<Node> parseUnary();
    std::unique_ptr<Node> parseTerm();
    std::unique_ptr<Node> parseExpression();
    std::unique_ptr<Node> parseAssignment();
    std::unique_ptr<Node> parseBraces();
    std::unique_ptr<Node> parseIfStatement();
    std::unique_ptr<Node> parseWhileStatement();
    std::unique_ptr<Node> parseStatement();
    std::unique_ptr<FunctionNode> parseFunction();
};

#endif  // PARSER_H
