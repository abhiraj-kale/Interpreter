#include "scanner.hpp"
#include <cctype>
#include <stdexcept> // For std::runtime_error

std::vector<Token> Scanner::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }

    tokens.emplace_back(TokenType::END_OF_FILE, "", std::any(), line);
    return tokens;
}

bool Scanner::isAtEnd() { return current >= source.length(); }

char Scanner::advance() { return source[current++]; }

void Scanner::addToken(TokenType type, std::any literal) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, literal, line);
}

void Scanner::addToken(TokenType type) {
    addToken(type, std::any());
}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

bool Scanner::isAlpha(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

bool Scanner::isDigit(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '+': addToken(TokenType::PLUS); break;
        case '-': addToken(TokenType::MINUS); break;
        case '*': addToken(TokenType::STAR); break;
        case '/': addToken(TokenType::SLASH); break;
        case '=': addToken(TokenType::EQUAL); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case ' ':
        case '\r':
        case '\t':
        case '\n': 
            break; 
        default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                throw std::runtime_error("Unexpected character: " + std::string(1, c));
            }
    }
}

void Scanner::number() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(source[current + 1])) {
        advance();
        while (isDigit(peek())) advance();
    }

    double value = std::stod(source.substr(start, current - start));
    addToken(TokenType::NUMBER, value);
}

void Scanner::identifier() {
    while (isAlphaNumeric(peek())) advance();
    std::string text = source.substr(start, current - start);

    if (text == "var") {
        addToken(TokenType::VAR);
    } else if(text == "let") {
        addToken(TokenType::LET);
    } else if (text == "print") {
        addToken(TokenType::PRINT);
    } else {
        addToken(TokenType::IDENTIFIER, text);
    }
}
