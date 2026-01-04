#ifndef LEXER_H
#define LEXER_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>


enum class TokenType {
    IF,
    ELSE,
    WHILE,
    IDENTIFIER,
    NUMBER,
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    SEMICOLON,
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    EQUALS,
    DOUBLE_EQUALS
};

struct Token {
    std::string_view lexeme;
    TokenType type;
};

class Lexer {
    std::string_view source;
    int start = 0;
    int cur = 0;

public:
    std::vector<Token> tokens;  // TODO later make private
    Lexer(std::string_view source) : source(source) {}
    bool EndReached();
    void GetToken();
    void PrintTokens();
};

#endif  // LEXER_H
