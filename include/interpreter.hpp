#pragma once
#include "parser.hpp"
#include <vector>

class Interpreter {
public:
    void interpret(const std::vector<ExprPtr>& expressions);
};