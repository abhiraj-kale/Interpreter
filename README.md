# C++ Expression Interpreter

A lightweight C++ interpreter that parses and evaluates arithmetic expressions with support for variables and operator precedence. It provides a simple REPL (Read-Eval-Print Loop) interface.

## Features

- Parses and evaluates arithmetic expressions: `+`, `-`, `*`, `/`
- Correct operator precedence and left-to-right associativity
- Variable assignment and usage: `x = 5`, `x + 2`
- REPL interface for interactive expression input
- Basic error handling (syntax errors, division by zero, undefined variables)

## Installation

### Prerequisites

- C++17 compatible compiler (GCC, Clang, MSVC)
- CMake version 3.16 or later

### Build Instructions

```bash
git clone https://github.com/your-username/interpreter.git
cd interpreter

# Create a build directory
mkdir -p out/build && cd out/build

# Configure the project
cmake --preset default

# Build the executable
cmake --build . --target Interpreter
