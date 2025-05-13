cmake_minimum_required(VERSION 3.16)
project(Interpreter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_executable(Interpreter
    main.cpp
    src/scanner.cpp
    src/parser.cpp
    src/interpreter.cpp
)
