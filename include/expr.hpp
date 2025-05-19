#ifndef EXPR_HPP
#define EXPR_HPP

#pragma once
#include <memory>
#include <string>
#include <vector>
#include <stdexcept>
#include "value.hpp"
#include "token.hpp"

struct Stmt;
struct FunctionStmt; 

struct Expr {
    virtual ~Expr() = default;
    virtual Value evaluate(Environment& env) = 0;
};

struct Literal : public Expr {
    Value value;

    Literal(double val) : value(val) {}
    Literal(const std::string& val) : value(val) {}

    Value evaluate(Environment&) override {
        return value;
    }
};

struct Variable : public Expr {
    std::string name;

    Variable(const std::string& name) : name(name) {}

    Value evaluate(Environment& env) override {
        auto it = env.find(name);
        if (it == env.end()) {
            throw std::runtime_error("Undefined variable: " + name);
        }
        return it->second;
    }
};

struct Assign : public Expr {
    std::string name;
    std::shared_ptr<Expr> valueExpr;

    Assign(const std::string& name, std::shared_ptr<Expr> value)
        : name(name), valueExpr(value) {}

    Value evaluate(Environment& env) override {
        Value val = valueExpr->evaluate(env);
        env[name] = val;
        return val;
    }
};

struct Binary : public Expr {
    std::shared_ptr<Expr> left;
    std::string op;
    std::shared_ptr<Expr> right;

    Binary(std::shared_ptr<Expr> left, const std::string& op, std::shared_ptr<Expr> right)
        : left(left), op(op), right(right) {}

    Value evaluate(Environment& env) override {
        Value l = left->evaluate(env);
        Value r = right->evaluate(env);

        if (op == "+") {
            if (std::holds_alternative<std::string>(l) && std::holds_alternative<std::string>(r)) {
                return std::get<std::string>(l) + std::get<std::string>(r);
            }
            if (std::holds_alternative<double>(l) && std::holds_alternative<double>(r)) {
                return std::get<double>(l) + std::get<double>(r);
            }
            throw std::runtime_error("Type error: '+' operator requires both operands of same type");
        }
        else if (op == "-") {
            if (std::holds_alternative<double>(l) && std::holds_alternative<double>(r)) {
                return std::get<double>(l) - std::get<double>(r);
            }
            throw std::runtime_error("Type error: '-' operator requires numbers");
        }
        else if (op == "*") {
            if (std::holds_alternative<double>(l) && std::holds_alternative<double>(r)) {
                return std::get<double>(l) * std::get<double>(r);
            }
            throw std::runtime_error("Type error: '*' operator requires numbers");
        }
        else if (op == "/") {
            if (std::holds_alternative<double>(l) && std::holds_alternative<double>(r)) {
                double divisor = std::get<double>(r);
                if (divisor == 0) throw std::runtime_error("Division by zero");
                return std::get<double>(l) / divisor;
            }
            throw std::runtime_error("Type error: '/' operator requires numbers");
        }
        else if (op == "==") {
            return l == r ? 1.0 : 0.0;
        }
        else if (op == "!=") {
            return l != r ? 1.0 : 0.0;
        }
        else if (op == "<") {
            if (std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
                return std::get<double>(l) < std::get<double>(r) ? 1.0 : 0.0;
            throw std::runtime_error("Type error: '<' requires numbers");
        }
        else if (op == "<=") {
            if (std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
                return std::get<double>(l) <= std::get<double>(r) ? 1.0 : 0.0;
            throw std::runtime_error("Type error: '<=' requires numbers");
        }
        else if (op == ">") {
            if (std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
                return std::get<double>(l) > std::get<double>(r) ? 1.0 : 0.0;
            throw std::runtime_error("Type error: '>' requires numbers");
        }
        else if (op == ">=") {
            if (std::holds_alternative<double>(l) && std::holds_alternative<double>(r))
                return std::get<double>(l) >= std::get<double>(r) ? 1.0 : 0.0;
            throw std::runtime_error("Type error: '>=' requires numbers");
        }

        throw std::runtime_error("Unknown operator: " + op);
    }
};
struct Unary : public Expr {
   Token op;
    std::shared_ptr<Expr> right;

    Unary(const Token& op, std::shared_ptr<Expr> right)
    : op(op), right(right) {}

    Value evaluate(Environment& env) override {
        Value val = right->evaluate(env);
        if (op.lexeme == "-") {
            if (std::holds_alternative<double>(val)) {
                return -std::get<double>(val);
            }
            throw std::runtime_error("Unary '-' requires a number.");
        } else if (op.lexeme == "!") {
            if (std::holds_alternative<double>(val)) {
                return std::get<double>(val) == 0.0 ? 1.0 : 0.0;
            }
            throw std::runtime_error("Unary '!' requires a number.");
        }
        throw std::runtime_error("Unknown unary operator.");
    }
};

struct Call : public Expr {
    std::string callee;
    std::vector<std::shared_ptr<Expr>> arguments;
    
    Call(std::string callee, std::vector<std::shared_ptr<Expr>> args); 

    Value evaluate(Environment& env) override;
};

#endif 

struct Postfix : public Expr {
    std::shared_ptr<Expr> operand;
    Token op;

    Postfix(std::shared_ptr<Expr> operand, Token op)
        : operand(std::move(operand)), op(std::move(op)) {}

    Value evaluate(Environment& env) override;
};
