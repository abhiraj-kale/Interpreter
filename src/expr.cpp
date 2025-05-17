#include "expr.hpp"
#include "stmt.hpp"
#include "expr.hpp"

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

    return {}; // default return value
}