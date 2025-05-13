#pragma once
#include "token.hpp"
#include <vector>
#include <string>

class Scanner {
    const std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    bool isAtEnd();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, std::any literal);
    void scanToken();
    void number();
    char peek();
    bool isDigit(char c);

public:
    explicit Scanner(std::string src) : source(std::move(src)) {}
    std::vector<Token> scanTokens();
};
