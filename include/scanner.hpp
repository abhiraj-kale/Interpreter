#pragma once
#include "token.hpp"
#include <string>
#include <vector>

class Scanner {
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;

    bool isAtEnd();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, std::any literal);
    char peek();
    bool isDigit(char c);
    bool isAlpha(char c);              
    bool isAlphaNumeric(char c);       
    void scanToken();
    void number();
    void identifier();                 

public:
    explicit Scanner(const std::string& source) : source(source) {}
    std::vector<Token> scanTokens();
};
