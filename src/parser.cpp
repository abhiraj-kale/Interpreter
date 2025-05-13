#include "parser.hpp"
#include <stdexcept>
#include "stmt.hpp" // Add this line to include the definition of VarStmt

std::shared_ptr<Stmt> Parser::parseStatement() {
    if (match({TokenType::VAR}) || match({TokenType::LET})) {
        std::string name = consume(TokenType::IDENTIFIER, "Expected variable name.").lexeme;
        consume(TokenType::EQUAL, "Expected '=' after variable name.");
        auto initializer = parseExpression();
        consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
        return std::make_shared<VarStmt>(name, initializer);
    }

    if (match({TokenType::PRINT})) {
        auto value = parseExpression();
        consume(TokenType::SEMICOLON, "Expected ';' after value.");
        return std::make_shared<PrintStmt>(value);
    }

    auto expr = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression.");
    return std::make_shared<ExpressionStmt>(expr);
}

std::shared_ptr<Expr> Parser::parseExpression() {
    return parseAssignment();
}

std::shared_ptr<Expr> Parser::parseAssignment() {
    auto expr = parseEquality();

    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        auto value = parseAssignment();

        if (auto var = std::dynamic_pointer_cast<Variable>(expr)) {
            return std::make_shared<Assign>(var->name, value);
        }

        throw std::runtime_error("Invalid assignment target.");
    }

    return expr;
}

std::shared_ptr<Expr> Parser::parseEquality() {
    auto expr = parseTerm();

    while (match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        auto right = parseTerm();
        expr = std::make_shared<Binary>(expr, op.lexeme, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::parseTerm() {
    auto expr = parseFactor();

    while (match({TokenType::STAR, TokenType::SLASH})) {
        Token op = previous();
        auto right = parseFactor();
        expr = std::make_shared<Binary>(expr, op.lexeme, right);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::parseFactor() {
    return parsePrimary();
}

std::shared_ptr<Expr> Parser::parsePrimary() {
    if (match({TokenType::NUMBER})) {
        return std::make_shared<Literal>(std::stod(previous().lexeme));
    }

    if (match({TokenType::IDENTIFIER})) {
        return std::make_shared<Variable>(previous().lexeme);
    }

    throw std::runtime_error("Expected expression.");
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (TokenType type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error(message);
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EOF_TOKEN;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}