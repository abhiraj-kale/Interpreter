#pragma once
#include <string>
#include <vector>
#include "token.hpp"
#include <any>

class Scanner {
public:
    explicit Scanner(const std::string& source) : source(source) {}

    std::vector<Token> scanTokens();

private:
    const std::string& source;
    std::vector<Token> tokens;
    size_t start = 0;
    size_t current = 0;
    int line = 1;

    bool isAtEnd();
    char advance();
    void addToken(TokenType type, std::any literal = {});
    char peek();
    bool match(char expected);

    void scanToken();
    void number();
    void identifier();
    void string();

    bool isDigit(char c);
    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
};
