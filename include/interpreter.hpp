#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <unordered_map>
#include <string>
#include <memory>
#include <vector>
#include "stmt.hpp"

class Interpreter {
public:
    void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);
private:
    std::unordered_map<std::string, double> environment;
};

#endif // INTERPRETER_HPP
