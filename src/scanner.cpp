#include "scanner.hpp"
#include <cctype>
#include <stdexcept>

std::vector<Token> Scanner::scanTokens() {
    tokens.clear();
    while (!isAtEnd()) {
        start = current;
        scanToken();
    }
    tokens.emplace_back(TokenType::END_OF_FILE, "", std::any(), line);
    return tokens;
}

bool Scanner::isAtEnd() {
    return current >= source.length();
}

char Scanner::advance() {
    return source[current++];
}

void Scanner::addToken(TokenType type, std::any literal) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, literal, line);
}

char Scanner::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

bool Scanner::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;
    current++;
    return true;
}

void Scanner::scanToken() {
    char c = advance();
    switch (c) {
        case '+': addToken(TokenType::PLUS); break;
        case '-': addToken(TokenType::MINUS); break;
        case '*': addToken(TokenType::STAR); break;
        case '/': addToken(TokenType::SLASH); break;
        case '=':
            addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
            break;
        case '!': addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
        case '<':
            addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
            break;
        case '>':
            addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
            break;
        case '"': string(); break;
        case ';': addToken(TokenType::SEMICOLON); break;
        case ',':
            tokens.emplace_back(TokenType::COMMA, ",", std::any(), line);
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;
        case '(': addToken(TokenType::LEFT_PAREN); break;
        case ')': addToken(TokenType::RIGHT_PAREN); break;
        case '{': addToken(TokenType::LEFT_BRACE); break;
        case '}': addToken(TokenType::RIGHT_BRACE); break;
        default:
            if (isDigit(c)) {
                number();
            } else if (isAlpha(c)) {
                identifier();
            } else {
                throw std::runtime_error("Unexpected character: " + std::string(1, c));
            }
            break;
    }
}

void Scanner::string() {
    while (peek() != '"' && !isAtEnd()) advance();

    if (isAtEnd())
        throw std::runtime_error("Unterminated string.");

    advance(); 

    std::string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
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

    if (text == "let") {
        addToken(TokenType::LET);
    } else if (text == "var") {
        addToken(TokenType::VAR);
    } else if (text == "print") {
        addToken(TokenType::PRINT);
    } else if (text == "true") {
        addToken(TokenType::TRUE);
    } else if (text == "false") {
        addToken(TokenType::FALSE);
    } 
    else if (text == "and") {
        addToken(TokenType::AND);
    } else if (text == "or") {
        addToken(TokenType::OR);
    } else if (text == "return") {
        addToken(TokenType::RETURN);
    } else if (text == "function") {
        addToken(TokenType::FUN);
    }    
    else if (text == "if") addToken(TokenType::IF);
    else if (text == "else") addToken(TokenType::ELSE);
    else if (text == "while") addToken(TokenType::WHILE);
    else {
        addToken(TokenType::IDENTIFIER, text);
    }
}

bool Scanner::isDigit(char c) {
    return std::isdigit(static_cast<unsigned char>(c));
}

bool Scanner::isAlpha(char c) {
    return std::isalpha(static_cast<unsigned char>(c)) || c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}
