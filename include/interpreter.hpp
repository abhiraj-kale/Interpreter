#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "value.hpp"

class Interpreter {
public:
    void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);
    Environment environment;
};


#endif 
