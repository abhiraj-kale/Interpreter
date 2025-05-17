#pragma once
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include "expr.hpp"
#include "value.hpp"

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
        for (auto& stmt : statements) {
            stmt->execute(env);
        }
    }
};

struct IfStmt : public Stmt {
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> thenBranch;
    std::shared_ptr<Stmt> elseBranch;

    IfStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> thenBr, std::shared_ptr<Stmt> elseBr = nullptr)
        : condition(cond), thenBranch(thenBr), elseBranch(elseBr) {}

    void execute(Environment& env) override {
        Value condVal = condition->evaluate(env);
        if (std::holds_alternative<double>(condVal) && std::get<double>(condVal) != 0.0) {
            thenBranch->execute(env);
        } else if (elseBranch) {
            elseBranch->execute(env);
        }
    }
};

struct WhileStmt : public Stmt {
    std::shared_ptr<Expr> condition;
    std::shared_ptr<Stmt> body;

    WhileStmt(std::shared_ptr<Expr> cond, std::shared_ptr<Stmt> body)
        : condition(cond), body(body) {}

    void execute(Environment& env) override {
        while (true) {
            Value condVal = condition->evaluate(env);
            if (!std::holds_alternative<double>(condVal) || std::get<double>(condVal) == 0.0)
                break;
            body->execute(env);
        }
    }
};

struct FunctionStmt : public Stmt, public std::enable_shared_from_this<FunctionStmt> {
    std::string name;
    std::vector<std::string> params;
    std::vector<std::shared_ptr<Stmt>> body;

    FunctionStmt(std::string name, std::vector<std::string> params, std::vector<std::shared_ptr<Stmt>> body)
        : name(std::move(name)), params(std::move(params)), body(std::move(body)) {}

    void execute(Environment& env) override {
    env[name] = std::static_pointer_cast<Stmt>(shared_from_this());
    }
};

struct ReturnStmt : public Stmt {
    std::shared_ptr<Expr> value;
    ReturnStmt(std::shared_ptr<Expr> value) : value(value) {}
    void execute(Environment& env) override {
        throw value->evaluate(env);
    }
};
