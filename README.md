# C++ Expression Interpreter

A lightweight C++ interpreter that parses and evaluates arithmetic expressions with support for variables, operator precedence, and basic statements. It provides a simple REPL (Read-Eval-Print Loop) interface.

## Architecture Overview

This interpreter is structured in the following phases:
- **Scanner (Lexer)** – Converts input strings into a list of tokens.
- **Parser** – Builds an Abstract Syntax Tree (AST) from the tokens.
- **Interpreter** – Walks the AST and evaluates expressions.
- **REPL** – A loop that reads user input, parses, evaluates, and prints results.

Each component is modular and easy to extend with new features like grouping, functions, or control flow.

## Features

- Parses and evaluates arithmetic expressions: `+`, `-`, `*`, `/`
- Correct operator precedence and left-to-right associativity
- **Variable assignment and usage**: `let x = 5`, `x + 2`, `var y = x + 10`
- **Support for both `let` and `var`** for variable declarations
- **Print statements**: `print x + y`
- **Semicolon-terminated statements**: `let x = 5; print x + 2;`
- REPL interface for interactive expression input
- Basic error handling (syntax errors, division by zero, undefined variables)

## Pre-built Binaries v0.1.0

A Windows .exe is available in the v0.1.0 Pre-release.

Download:
➡️ [Interpreter.exe](https://github.com/abhiraj-kale/Interpreter/releases/tag/v0.1.1)

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
