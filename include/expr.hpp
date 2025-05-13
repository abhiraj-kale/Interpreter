#ifndef EXPR_HPP
#define EXPR_HPP

#include <memory>
#include <string>
#include <stdexcept>
#include <unordered_map>

struct Expr {
    virtual ~Expr() = default;
    virtual double evaluate(std::unordered_map<std::string, double>& env) = 0;
};

struct Literal : public Expr {
    double value;
    Literal(double value) : value(value) {}
    double evaluate(std::unordered_map<std::string, double>&) override {
        return value;
    }
};

struct Variable : public Expr {
    std::string name;
    Variable(const std::string& name) : name(name) {}
    double evaluate(std::unordered_map<std::string, double>& env) override {
        return env.at(name);
    }
};

struct Assign : public Expr {
    std::string name;
    std::shared_ptr<Expr> value;
    Assign(const std::string& name, std::shared_ptr<Expr> value) : name(name), value(value) {}
    double evaluate(std::unordered_map<std::string, double>& env) override {
        double val = value->evaluate(env);
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

    double evaluate(std::unordered_map<std::string, double>& env) override {
        double l = left->evaluate(env);
        double r = right->evaluate(env);
        if (op == "+") return l + r;
        if (op == "-") return l - r;
        if (op == "*") return l * r;
        if (op == "/") return l / r;
        throw std::runtime_error("Unknown operator: " + op);
        return 0;
    }
};

#endif 
