#include "parser.hpp"
#include <stdexcept>

Parser::Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

ExprPtr Parser::parseExpr() {
    return expression();
}

ExprPtr Parser::expression() {
    return term();
}

ExprPtr Parser::term() {
    ExprPtr expr = factor();

    while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        Token op = previous();
        ExprPtr right = factor();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

ExprPtr Parser::factor() {
    ExprPtr expr = primary();

    while (match(TokenType::STAR) || match(TokenType::SLASH)) {
        Token op = previous();
        ExprPtr right = primary();
        expr = std::make_shared<Binary>(expr, op, right);
    }

    return expr;
}

ExprPtr Parser::primary() {
    if (match(TokenType::NUMBER)) {
        double value = std::any_cast<double>(previous().literal);
        return std::make_shared<Literal>(value);
    }

    throw std::runtime_error("Expected expression.");
}

bool Parser::match(TokenType type) {
    if (isAtEnd()) return false;
    if (peek().type != type) return false;
    advance();
    return true;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::END_OF_FILE;
}

// === Binary Expression Evaluation ===
double Binary::evaluate() {
    double leftVal = left->evaluate();
    double rightVal = right->evaluate();

    switch (op.type) {
        case TokenType::PLUS: return leftVal + rightVal;
        case TokenType::MINUS: return leftVal - rightVal;
        case TokenType::STAR: return leftVal * rightVal;
        case TokenType::SLASH:
            if (rightVal == 0) throw std::runtime_error("Division by zero.");
            return leftVal / rightVal;
        default:
            throw std::runtime_error("Invalid binary operator.");
    }
}
