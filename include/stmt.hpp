#ifndef STMT_HPP
#define STMT_HPP

#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>
#include "expr.hpp"

struct Stmt {
    virtual ~Stmt() = default;
    virtual void execute(std::unordered_map<std::string, double>& env) = 0;
};

struct ExpressionStmt : public Stmt {
    std::shared_ptr<Expr> expression;
    ExpressionStmt(std::shared_ptr<Expr> expression) : expression(expression) {}
    void execute(std::unordered_map<std::string, double>& env) override {
        expression->evaluate(env);
    }
};

struct PrintStmt : public Stmt {
    std::shared_ptr<Expr> expression;
    PrintStmt(std::shared_ptr<Expr> expression) : expression(expression) {}
    void execute(std::unordered_map<std::string, double>& env) override {
        std::cout << expression->evaluate(env) << std::endl;
    }
};

struct VarStmt : public Stmt {
    std::string name;
    std::shared_ptr<Expr> initializer;
    VarStmt(const std::string& name, std::shared_ptr<Expr> initializer)
        : name(name), initializer(initializer) {}
    void execute(std::unordered_map<std::string, double>& env) override {
        env[name] = initializer->evaluate(env);
    }
};

#endif // STMT_HPP
