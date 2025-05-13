#pragma once
#include <string>
#include <any>
#include <iostream>

enum class TokenType {
    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN,
    PLUS, MINUS, STAR, SLASH,

    // Literals
    IDENTIFIER, STRING, NUMBER,

    // Keywords
    PRINT,

    END_OF_FILE
};

class Token {
public:
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;

    Token(TokenType type, std::string lexeme, std::any literal, int line)
        : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line) {}

    std::string toString() const {
        return lexeme;
    }
};