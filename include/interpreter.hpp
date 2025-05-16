#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <unordered_map>
#include <string>
#include <variant>
#include <memory>
#include <vector>

using Value = std::variant<double, std::string>;
using Environment = std::unordered_map<std::string, Value>;
struct Stmt;

class Interpreter {
public:
    void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);

private:
    Environment environment;
};

#endif 
