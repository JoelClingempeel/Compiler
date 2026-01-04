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

void Parser::consume(TokenType type) {
    Token token = advance();
    if (token.type != type) {
        throw std::runtime_error("Missing )");
    }
}

std::unique_ptr<Node> Parser::parseFactor() {
    if (match(TokenType::NUMBER) || match(TokenType::IDENTIFIER)) {
        return std::make_unique<Node>(previous(), nullptr, nullptr);
    }
    // TODO Handle parentheses.
    if (match(TokenType::LEFT_PAREN)) {
        auto node = parseExpression();
        consume(TokenType::RIGHT_PAREN);
        return node;
    }
    throw std::runtime_error("Missing number of identifier");
}

std::unique_ptr<Node> Parser::parseTerm() {
    auto left = parseFactor();
    while (match(TokenType::MULTIPLY) || match(TokenType::DIVIDE)) {
        Token operation = previous();
        auto right = parseFactor();
        left = std::make_unique<Node>(operation, std::move(left), std::move(right));
    }
    return left;
}

std::unique_ptr<Node> Parser::parseExpression() {
    auto left = parseTerm();
    while (match(TokenType::ADD) || match(TokenType::SUBTRACT)) {
        Token operation = previous();
        auto right = parseTerm();
        left = std::make_unique<Node>(operation, std::move(left), std::move(right));
    }
    return left;
}
