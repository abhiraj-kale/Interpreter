#pragma once
#include "token.hpp"
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

class Expr;
using ExprPtr = std::shared_ptr<Expr>;

class Expr {
public:
    virtual double evaluate(std::unordered_map<std::string, double>& env) = 0;
    virtual ~Expr() = default;
};

class Literal : public Expr {
    double value;
public:
    explicit Literal(double val) : value(val) {}
    double evaluate(std::unordered_map<std::string, double>&) override {
        return value;
    }
};

class Variable : public Expr {
public:
    std::string name;
    explicit Variable(std::string n) : name(std::move(n)) {}
    double evaluate(std::unordered_map<std::string, double>& env) override {
        if (!env.count(name)) throw std::runtime_error("Undefined variable: " + name);
        return env[name];
    }
};

class Assign : public Expr {
    std::string name;
    ExprPtr value;
public:
    Assign(std::string n, ExprPtr v) : name(std::move(n)), value(std::move(v)) {}
    double evaluate(std::unordered_map<std::string, double>& env) override {
        double val = value->evaluate(env);
        env[name] = val;
        return val;
    }
};

class Binary : public Expr {
    ExprPtr left;
    Token op;
    ExprPtr right;
public:
    Binary(ExprPtr l, Token o, ExprPtr r) : left(std::move(l)), op(std::move(o)), right(std::move(r)) {}
    double evaluate(std::unordered_map<std::string, double>& env) override;
};

class Parser {
    const std::vector<Token>& tokens;
    int current = 0;

    ExprPtr expression();
    ExprPtr assignment();
    ExprPtr term();
    ExprPtr factor();
    ExprPtr primary();

    bool match(TokenType type);
    Token advance();
    Token peek();
    Token previous();
    bool isAtEnd();

public:
    explicit Parser(const std::vector<Token>& tokens);
    ExprPtr parseExpr();
};
