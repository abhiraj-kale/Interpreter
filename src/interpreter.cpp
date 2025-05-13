#include "interpreter.hpp"
#include <iostream>

void Interpreter::interpret(const std::vector<ExprPtr>& expressions) {
    for (const auto& expr : expressions) {
        std::cout << expr->evaluate() << std::endl;
    }
}