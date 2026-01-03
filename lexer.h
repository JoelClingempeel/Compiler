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
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    EQUALS
};

struct Token {
    std::string_view lexeme;
    TokenType type;
};

class Lexer {
    std::string_view source;
    std::vector<Token> tokens;
    int start = 0;
    int cur = 0;

public:
    Lexer(std::string_view source) : source(source) {}
    bool EndReached();
    void GetToken();
    void PrintTokens();
};

#endif  // LEXER_H
