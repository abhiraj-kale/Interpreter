#pragma once

#include "token.hpp"
#include <memory>
#include <vector>

class Expr;
using ExprPtr = std::shared_ptr<Expr>;

// === Abstract Expression ===
class Expr {
public:
    virtual double evaluate() = 0;
    virtual ~Expr() = default;
};

// === Literal Expression ===
class Literal : public Expr {
    double value;
public:
    explicit Literal(double val) : value(val) {}
    double evaluate() override { return value; }
};

// === Binary Expression ===
class Binary : public Expr {
    ExprPtr left;
    Token op;
    ExprPtr right;
public:
    Binary(ExprPtr left, Token op, ExprPtr right)
        : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

    double evaluate() override;
};

// === Parser ===
class Parser {
    const std::vector<Token>& tokens;
    int current = 0;

    bool isAtEnd();
    Token advance();
    Token peek();
    Token previous();
    bool match(TokenType type);

    ExprPtr expression();
    ExprPtr term();     // + -
    ExprPtr factor();   // * /
    ExprPtr primary();  // numbers

public:
    explicit Parser(const std::vector<Token>& tokens);
    ExprPtr parseExpr();  
};
