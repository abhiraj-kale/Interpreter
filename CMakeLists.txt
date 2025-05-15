cmake_minimum_required(VERSION 3.16)
project(CPPInterpreter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(SOURCES
    main.cpp
    src/scanner.cpp
    src/parser.cpp
    src/interpreter.cpp
)

add_executable(Interpreter ${SOURCES})