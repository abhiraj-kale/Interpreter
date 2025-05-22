#include <gtest/gtest.h>
#include "scanner.hpp"
#include "parser.hpp"
#include "expr.hpp"
#include "stmt.hpp"

std::shared_ptr<Stmt> parseSingleStatement(const std::string& source) {
    Scanner scanner(source);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    auto stmts = parser.parse();
    EXPECT_EQ(stmts.size(), 1);
    return stmts[0];
}

TEST(ParserTest, ParsesLetDeclaration) {
    auto stmt = parseSingleStatement("let x = 42;");

    auto varStmt = std::dynamic_pointer_cast<VarStmt>(stmt);
    ASSERT_NE(varStmt, nullptr);
    EXPECT_EQ(varStmt->name, "x");

    auto literal = std::dynamic_pointer_cast<Literal>(varStmt->initializer);
    ASSERT_NE(literal, nullptr);
    EXPECT_EQ(std::get<double>(literal->value), 42.0);
}

TEST(ParserTest, ParsesPrintStatement) {
    auto stmt = parseSingleStatement("print 2 + 3;");

    auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt);
    ASSERT_NE(printStmt, nullptr);

    auto binary = std::dynamic_pointer_cast<Binary>(printStmt->expression);
    ASSERT_NE(binary, nullptr);
    EXPECT_EQ(binary->op, "+");
}

TEST(ParserTest, ParsesWhileLoop) {
    auto stmt = parseSingleStatement("while (x < 10) print x;");

    auto whileStmt = std::dynamic_pointer_cast<WhileStmt>(stmt);
    ASSERT_NE(whileStmt, nullptr);

    auto condition = std::dynamic_pointer_cast<Binary>(whileStmt->condition);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->op, "<");
}

TEST(ParserTest, ParsesFunctionDeclaration) {
    auto stmt = parseSingleStatement("function add(a, b) { return a + b; }");

    auto funcStmt = std::dynamic_pointer_cast<FunctionStmt>(stmt);
    ASSERT_NE(funcStmt, nullptr);
    EXPECT_EQ(funcStmt->name, "add");
    ASSERT_EQ(funcStmt->params.size(), 2);
    EXPECT_EQ(funcStmt->params[0], "a");
    EXPECT_EQ(funcStmt->params[1], "b");

    ASSERT_EQ(funcStmt->body.size(), 1);
    auto retStmt = std::dynamic_pointer_cast<ReturnStmt>(funcStmt->body[0]);
    ASSERT_NE(retStmt, nullptr);
}
