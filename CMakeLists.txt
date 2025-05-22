cmake_minimum_required(VERSION 3.16)
project(CPPInterpreter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

set(INTERPRETER_SOURCES
    main.cpp
    src/scanner.cpp
    src/parser.cpp
    src/interpreter.cpp
    src/expr.cpp
)

add_executable(Interpreter main.cpp ${INTERPRETER_SOURCES})

add_subdirectory(external/googletest)

enable_testing()

set(TEST_SOURCES
    test/scanner_test.cpp
    test/parser_test.cpp
    test/interpreter_test.cpp
    test/expr_test.cpp
)

add_executable(InterpreterTests ${TEST_SOURCES} ${INTERPRETER_SOURCES})

target_link_libraries(InterpreterTests
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(InterpreterTests)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(Interpreter PRIVATE --coverage -O0 -g)
    target_link_options(Interpreter PRIVATE --coverage)
    target_compile_options(InterpreterTests PRIVATE --coverage -O0 -g)
    target_link_options(InterpreterTests PRIVATE --coverage)
endif()
