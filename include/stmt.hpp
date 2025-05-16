#pragma once
#include <memory>
#include <string>
#include <iostream>
#include <unordered_map>
#include <variant>
#include "expr.hpp"

using Value = std::variant<double, std::string>;
using Environment = std::unordered_map<std::string, Value>;

struct Stmt {
    virtual ~Stmt() = default;
    virtual void execute(Environment& env) = 0;
};

struct ExpressionStmt : public Stmt {
    std::shared_ptr<Expr> expression;
    ExpressionStmt(std::shared_ptr<Expr> expression) : expression(expression) {}
    void execute(Environment& env) override {
        expression->evaluate(env);
    }
};

struct PrintStmt : public Stmt {
    std::shared_ptr<Expr> expression;
    PrintStmt(std::shared_ptr<Expr> expression) : expression(expression) {}
    void execute(Environment& env) override {
        Value val = expression->evaluate(env);
        if (std::holds_alternative<double>(val))
            std::cout << std::get<double>(val) << std::endl;
        else if (std::holds_alternative<std::string>(val))
            std::cout << std::get<std::string>(val) << std::endl;
    }
};

struct VarStmt : public Stmt {
    std::string name;
    std::shared_ptr<Expr> initializer;
    VarStmt(const std::string& name, std::shared_ptr<Expr> initializer)
        : name(name), initializer(initializer) {}
    void execute(Environment& env) override {
        env[name] = initializer->evaluate(env);
    }
};

struct BlockStmt : public Stmt {
    std::vector<std::shared_ptr<Stmt>> statements;
    BlockStmt(std::vector<std::shared_ptr<Stmt>> stmts) : statements(std::move(stmts)) {}
    void execute(Environment& env) override {
        Environment localEnv = env; 
        for (auto& stmt : statements) {
            stmt->execute(localEnv);
        }
    }
};