#include "lexer.h"


std::string_view TokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::IF:             return "IF";
        case TokenType::ELSE:           return "ELSE";
        case TokenType::WHILE:          return "WHILE";
        case TokenType::IDENTIFIER:     return "IDENTIFIER";
        case TokenType::NUMBER:         return "NUMBER";
        case TokenType::ADD:            return "ADD";
        case TokenType::SUBTRACT:       return "SUBTRACT";
        case TokenType::MULTIPLY:       return "MULTIPLY";
        case TokenType::DIVIDE:         return "DIVIDE";
        case TokenType::SEMICOLON:      return "SEMICOLON";
        case TokenType::LEFT_PAREN:     return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN:    return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE:     return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE:    return "RIGHT_BRACE";
        case TokenType::EQUALS:         return "EQUALS";
        case TokenType::LESS:           return "LESS";
        case TokenType::LESS_EQUALS:    return "LESS_EQUALS";
        case TokenType::GREATER:        return "GREATER";
        case TokenType::GREATER_EQUALS: return "GREATER_EQUALS";
        case TokenType::DOUBLE_EQUALS:  return "DOUBLE_EQUALS";
        case TokenType::NOT_EQUALS:     return "NOT_EQUALS";
        case TokenType::FUN:            return "FUN";
        case TokenType::COMMA:          return "COMMA";
        case TokenType::RETURN:         return "RETURN";
        default:                        return "UNKNOWN_TOKEN";
    }
}

std::unordered_map<std::string_view, TokenType> keywords = {
    {"if", TokenType::IF},
    {"else", TokenType::ELSE},
    {"while", TokenType::WHILE},
    {"fun", TokenType::FUN},
    {"return", TokenType::RETURN}
};

bool Lexer::EndReached() {
    return cur >= source.size();
}

void Lexer::GetToken(){
    std::string_view lexeme;
    TokenType type;
    start = cur;

    if (isalpha(source[cur])) {
        while (isalpha(source[cur])) {
            cur++;
        }
        lexeme = source.substr(start, cur - start);
        if (keywords.count(lexeme)) {
            type = keywords[lexeme];
        } else {
            type = TokenType::IDENTIFIER;
        }
        tokens.push_back({lexeme, type});
    } else if (isdigit(source[cur])) {
        while (isdigit(source[cur])) {
            cur++;
        }
        lexeme = source.substr(start, cur - start);
        type = TokenType::NUMBER;
        tokens.push_back({lexeme, type});
    } else if (source[cur] == ' ' || source[cur] == '\n') {
        cur++;
    } else {
        switch (source[cur]) {
            case '+':
                lexeme = source.substr(start, 1);
                type = TokenType::ADD;
                break;

            case '-':
                lexeme = source.substr(start, 1);
                type = TokenType::SUBTRACT;
                break;

            case '*':
                lexeme = source.substr(start, 1);
                type = TokenType::MULTIPLY;
                break;

            case '/':
                lexeme = source.substr(start, 1);
                type = TokenType::DIVIDE;
                break;

            case ';':
                lexeme = source.substr(start, 1);
                type = TokenType::SEMICOLON;
                break;

            case '(':
                lexeme = source.substr(start, 1);
                type = TokenType::LEFT_PAREN;
                break;

            case ')':
                lexeme = source.substr(start, 1);
                type = TokenType::RIGHT_PAREN;
                break;

            case '{':
                lexeme = source.substr(start, 1);
                type = TokenType::LEFT_BRACE;
                break;

            case '}':
                lexeme = source.substr(start, 1);
                type = TokenType::RIGHT_BRACE;
                break;

            case '=':
                if (cur + 1 < source.size() && source[cur + 1] == '=') {
                    lexeme = source.substr(start, 2);
                    type = TokenType::DOUBLE_EQUALS;
                    cur++;
                } else {
                    lexeme = source.substr(start, 1);
                    type = TokenType::EQUALS;
                }
                break;

            case '<':
                if (cur + 1 < source.size() && source[cur + 1] == '=') {
                    lexeme = source.substr(start, 2);
                    type = TokenType::LESS_EQUALS;
                    cur++;
                } else {
                    lexeme = source.substr(start, 1);
                    type = TokenType::LESS;
                }
                break;

            case '>':
                if (cur + 1 < source.size() && source[cur + 1] == '=') {
                    lexeme = source.substr(start, 2);
                    type = TokenType::GREATER_EQUALS;
                    cur++;
                } else {
                    lexeme = source.substr(start, 1);
                    type = TokenType::GREATER;
                }
                break;
            
             case '!':
                if (cur + 1 < source.size() && source[cur + 1] == '=') {
                    lexeme = source.substr(start, 2);
                    type = TokenType::NOT_EQUALS;
                    cur++;
                }
                break;

            case ',':
                lexeme = source.substr(start, 1);
                type = TokenType::COMMA;
                break;
        }
        tokens.push_back({lexeme, type});
        cur++;
    }
}

void Lexer::PrintTokens() {
    for (auto token : tokens) {
        std::cout << token.lexeme << " " << TokenTypeToString(token.type) << std::endl;
    }
}
