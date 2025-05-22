#include <gtest/gtest.h>
#include <sstream>
#include "scanner.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

std::vector<std::shared_ptr<Stmt>> parseSource(const std::string& source) {
    Scanner scanner(source);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    return parser.parse();
}

class StdoutCapture {
    std::streambuf* old_buf;
    std::stringstream ss;
public:
    void start() {
        old_buf = std::cout.rdbuf(ss.rdbuf());
    }
    std::string stop() {
        std::cout.rdbuf(old_buf);
        return ss.str();
    }
};

TEST(InterpreterTest, EvaluatesArithmeticExpression) {
    Interpreter interpreter;
    auto stmts = parseSource("print 1 + 2 * 3;");

    StdoutCapture capture;
    capture.start();
    interpreter.interpret(stmts);
    auto output = capture.stop();

    EXPECT_EQ(output, "7\n");
}

TEST(InterpreterTest, HandlesVariableDeclarationAndUsage) {
    Interpreter interpreter;
    auto stmts = parseSource(R"(
        let x = 10;
        print x + 5;
    )");

    StdoutCapture capture;
    capture.start();
    interpreter.interpret(stmts);
    auto output = capture.stop();

    EXPECT_EQ(output, "15\n");
}

TEST(InterpreterTest, HandlesWhileLoop) {
    Interpreter interpreter;
    auto stmts = parseSource(R"(
        let i = 0;
        while (i < 3) {
            print i;
            i = i + 1;
        }
    )");

    StdoutCapture capture;
    capture.start();
    interpreter.interpret(stmts);
    auto output = capture.stop();

    EXPECT_EQ(output, "0\n1\n2\n");
}

TEST(InterpreterTest, HandlesFunctionDefinitionAndCall) {
    Interpreter interpreter;
    auto stmts = parseSource(R"(
        function add(a, b) {
            return a + b;
        }
        print add(5, 7);
    )");

    StdoutCapture capture;
    capture.start();
    interpreter.interpret(stmts);
    auto output = capture.stop();

    EXPECT_EQ(output, "12\n");
}
