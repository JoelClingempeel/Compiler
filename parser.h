#ifndef PARSER_H
#define PARSER_H

#include "lexer.h"
#include "parser.h"


class Parser {
    int cur = 0;
    std::vector<Token> tokens;

    Token peek();
    Token previous();
    Token advance();
    bool endReached();
    bool match(TokenType type);

public:
    Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}
};

#endif  // PARSER_H
