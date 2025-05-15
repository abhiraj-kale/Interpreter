# C++ Expression Interpreter

A lightweight C++ interpreter that parses and evaluates arithmetic and comparison expressions with support for variables, string literals, and basic statements. It provides a simple REPL (Read-Eval-Print Loop) interface for interactive execution.

---

## Architecture Overview

This interpreter is structured in the following phases:

- **Scanner (Lexer)** – Converts input strings into a list of tokens.
- **Parser** – Builds an Abstract Syntax Tree (AST) from the tokens.
- **Interpreter** – Walks the AST and evaluates expressions.
- **REPL** – A loop that reads user input, parses, evaluates, and prints results.

Each component is modular and easy to extend with features like functions, control flow, and more.

---

## Features

- **Arithmetic operators**: `+`, `-`, `*`, `/`
- **Comparison operators**: `==`, `!=`, `<`, `<=`, `>`, `>=`
- **String literals**: `"hello"`, `"x = " + x`
- **Variable assignment and usage**:
  - `let x = 10;`
  - `var y = x + 5;`
- **Print statements**:
  - `print x + y;`
  - `print "Result: " + x;`
- **Semicolon-terminated statements**
- **REPL interface** for interactive command input
- **Error handling** for:
  - Syntax errors
  - Undefined variables
  - Division by zero
  - Unterminated strings

---

## How to Use (via REPL)

When you run the interpreter executable, you'll see a `>` prompt.

### Examples:

```txt
> let x = 10;
> var y = x * 2;
> print x + y;
30
> print "x = " + x;
x = 10
> print x == 10;
1
```
## Pre-built Binaries v0.1.2

A Windows .exe is available in the v0.1.2 release.

Download:
➡️ [Interpreter.exe](https://github.com/abhiraj-kale/Interpreter/releases/tag/v0.1.2)

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
```