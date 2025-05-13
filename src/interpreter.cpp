#include "interpreter.hpp"
#include "stmt.hpp"

void Interpreter::interpret(const std::vector<std::shared_ptr<Stmt>>& statements) {
    for (const auto& stmt : statements) {
        stmt->execute(environment);
    }
}
