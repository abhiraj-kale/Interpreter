#ifndef EXPR_HPP
#define EXPR_HPP

#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <stdexcept>
#include "interpreter.hpp"

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

        // Handle string concatenation with '+'
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
        // Add comparison operators
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

#endif // EXPR_HPP
