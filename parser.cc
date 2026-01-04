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
        std::string error = "Missing ";
        error += TokenTypeToString(type);
        throw std::runtime_error(error);
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

std::unique_ptr<Node> Parser::parseUnary() {
    if (match(TokenType::SUBTRACT)) {
        Token operation = previous();
        auto right = parseFactor();
        return std::make_unique<Node>(operation, nullptr, std::move(right));
    }
    return parseFactor();
}

std::unique_ptr<Node> Parser::parseTerm() {
    auto left = parseUnary();
    while (match(TokenType::MULTIPLY) || match(TokenType::DIVIDE)) {
        Token operation = previous();
        auto right = parseUnary();
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

std::unique_ptr<Node> Parser::parseAssignment() {
    auto left = parseExpression();
    if (match(TokenType::EQUALS)) {
        if (left->token.type == TokenType::IDENTIFIER) {
            Token operation = previous();
            auto right = parseExpression();
            return std::make_unique<Node>(operation, std::move(left), std::move(right));
        } else {
            throw std::runtime_error("Invalid assignment");
        }
    }
    return left;
}

std::unique_ptr<Node> Parser::parseBraces() {
    Token token({"{", TokenType::LEFT_BRACE});
    std::vector<std::unique_ptr<Node>> statement_nodes;

    consume(TokenType::LEFT_BRACE);
    while (peek().type != TokenType::RIGHT_BRACE) {
        statement_nodes.push_back(parseAssignment());
        consume(TokenType::SEMICOLON);
    }
    consume(TokenType::RIGHT_BRACE);
    return std::make_unique<Node>(token, std::move(statement_nodes));
}

std::unique_ptr<Node> Parser::parseIfStatement() {
    // TODO Add support for else and else-if.
    if (match(TokenType::IF)) {
        Token token = previous();
        consume(TokenType::LEFT_PAREN);
        auto left = parseExpression();
        consume(TokenType::RIGHT_PAREN);
        auto right = parseBraces();
        return std::make_unique<Node>(token, std::move(left), std::move(right));
    }
    throw std::runtime_error("Invalid if statement");
}

std::unique_ptr<Node> Parser::parseWhileStatement() {
    if (match(TokenType::WHILE)) {
        Token token = previous();
        consume(TokenType::LEFT_PAREN);
        auto left = parseExpression();
        consume(TokenType::RIGHT_PAREN);
        auto right = parseBraces();
        return std::make_unique<Node>(token, std::move(left), std::move(right));
    }
    throw std::runtime_error("Invalid while statement");
}

std::unique_ptr<Node> Parser::parseStatement() {
    if (peek().type == TokenType::IF) {
        return parseIfStatement();
    } else if (peek().type == TokenType::WHILE) {
        return parseWhileStatement();
    } else if (peek().type == TokenType::LEFT_BRACE) {
        return parseBraces();
    }
    auto out = parseAssignment();
    consume(TokenType::SEMICOLON);
    return out;
}
