#include "parser.h"


Token Parser::peek() {
    return tokens[cur];
}

Token Parser::previous() {
    return tokens[cur - 1];
}

Token Parser::advance() {
    cur++;
    return tokens[cur - 1];
}

bool Parser::endReached() {
    return cur == tokens.size();
}

bool Parser::match(TokenType type) {
    if (peek().type == type) {
        advance();
        return true;
    }
    return false;
}
