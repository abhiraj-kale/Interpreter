// value.hpp
#pragma once
#include <variant>
#include <string>
#include <memory>
#include <unordered_map>

struct Stmt;  // forward-declared
using Value = std::variant<double, std::string, std::shared_ptr<Stmt>>;
using Environment = std::unordered_map<std::string, Value>;
