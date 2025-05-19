#include "token.hpp"
#include "expr.hpp"
#include "stmt.hpp"
#include <memory>
#include <stdexcept>

Call::Call(std::string callee, std::vector<std::shared_ptr<Expr>> args)
    : callee(std::move(callee)), arguments(std::move(args)) {}

Value Call::evaluate(Environment& env) {
    auto it = env.find(callee);
    if (it == env.end()) throw std::runtime_error("Undefined function: " + callee);

    if (!std::holds_alternative<std::shared_ptr<Stmt>>(it->second))
        throw std::runtime_error("Value is not a function: " + callee);

    auto fn = std::get<std::shared_ptr<Stmt>>(it->second);
    auto function = std::dynamic_pointer_cast<FunctionStmt>(fn);
    if (!function) throw std::runtime_error("Value is not a FunctionStmt: " + callee);

    Environment localEnv = env;
    for (size_t i = 0; i < function->params.size(); ++i) {
        localEnv[function->params[i]] = arguments[i]->evaluate(env);
    }

    try {
        for (const auto& stmt : function->body)
            stmt->execute(localEnv);
    } catch (const Value& returnVal) {
        return returnVal;
    }

    return {};
}

Value Postfix::evaluate(Environment& env) {
    auto var = std::dynamic_pointer_cast<Variable>(operand);
    if (!var) {
        throw std::runtime_error("Postfix operator must be applied to a variable.");
    }

    auto it = env.find(var->name);
    if (it == env.end()) {
        throw std::runtime_error("Undefined variable '" + var->name + "'.");
    }

    Value current = it->second;

    if (!std::holds_alternative<double>(current)) {
        throw std::runtime_error("Postfix operators can only be applied to numbers.");
    }

    double val = std::get<double>(current);

    if (this->op.type == TokenType::INCREMENT) {
        env[var->name] = val + 1;
        return val; 
    } else if (this->op.type == TokenType::DECREMENT) {
        env[var->name] = val - 1;
        return val; 
    }
    throw std::runtime_error("Unknown postfix operator.");
}
