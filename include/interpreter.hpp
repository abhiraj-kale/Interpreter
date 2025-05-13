#pragma once
#include "parser.hpp"
#include <unordered_map>

class Interpreter {
    std::unordered_map<std::string, double> environment;
public:
    void interpret(const std::vector<ExprPtr>& expressions);
};