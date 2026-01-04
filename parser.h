#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "memory.h"
#include "parser.h"


struct Node {
    Token token;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    Node(Token token, std::unique_ptr<Node> left, std::unique_ptr<Node> right)
        : token(token), left(std::move(left)), right(std::move(right)) {}
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
};

#endif  // PARSER_H
