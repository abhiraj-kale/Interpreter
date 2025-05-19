#pragma once
#include <string>
#include <any>

enum class TokenType {
    LEFT_PAREN, RIGHT_PAREN,
    PLUS, MINUS, STAR, SLASH,
    EQUAL, SEMICOLON,
    EQUAL_EQUAL, BANG, BANG_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL, COMMA,
    RIGHT_BRACE, LEFT_BRACE,
    TRUE, FALSE,
    IF, ELSE, WHILE, FOR,
    PLUS_PLUS, MINUS_MINUS,
    INCREMENT, DECREMENT,
    IDENTIFIER, STRING, NUMBER,
    FUN, RETURN, AND, OR,
    LET, VAR, PRINT,
    END_OF_FILE,
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;

    Token(TokenType type, const std::string& lexeme, std::any literal, int line)
        : type(type), lexeme(lexeme), literal(std::move(literal)), line(line) {}
};
