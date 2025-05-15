#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include <unordered_map>
#include <string>
#include <variant>
#include <memory>
#include <vector>

// A variant type that holds either double or string values
using Value = std::variant<double, std::string>;

// Environment is a map from variable names to their values
using Environment = std::unordered_map<std::string, Value>;

// Forward declaration of Stmt (pointer only)
struct Stmt;

class Interpreter {
public:
    void interpret(const std::vector<std::shared_ptr<Stmt>>& statements);

private:
    Environment environment;
};

#endif // INTERPRETER_HPP
