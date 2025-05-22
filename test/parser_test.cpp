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

std::vector<std::shared_ptr<Stmt>> parseStatements(const std::string& source) {
    Scanner scanner(source);
    auto tokens = scanner.scanTokens();
    Parser parser(tokens);
    return parser.parse();
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

TEST(ParserTest, ParsesVarDeclaration) {
    auto stmt = parseSingleStatement("var y = 3.14;");
    auto varStmt = std::dynamic_pointer_cast<VarStmt>(stmt);
    ASSERT_NE(varStmt, nullptr);
    EXPECT_EQ(varStmt->name, "y");
    auto literal = std::dynamic_pointer_cast<Literal>(varStmt->initializer);
    ASSERT_NE(literal, nullptr);
    EXPECT_EQ(std::get<double>(literal->value), 3.14);
}

TEST(ParserTest, ParsesPrintStatement) {
    auto stmt = parseSingleStatement("print 2 + 3;");
    auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt);
    ASSERT_NE(printStmt, nullptr);
    auto binary = std::dynamic_pointer_cast<Binary>(printStmt->expression);
    ASSERT_NE(binary, nullptr);
    EXPECT_EQ(binary->op, "+");
}

TEST(ParserTest, ParsesBlockStatement) {
    auto stmt = parseSingleStatement("{ let x = 1; print x; }");
    auto blockStmt = std::dynamic_pointer_cast<BlockStmt>(stmt);
    ASSERT_NE(blockStmt, nullptr);
    ASSERT_EQ(blockStmt->statements.size(), 2);
}

TEST(ParserTest, ParsesIfElseStatement) {
    auto stmt = parseSingleStatement("if (x > 0) print x; else print 0;");
    auto ifStmt = std::dynamic_pointer_cast<IfStmt>(stmt);
    ASSERT_NE(ifStmt, nullptr);
    auto thenPrint = std::dynamic_pointer_cast<PrintStmt>(ifStmt->thenBranch);
    ASSERT_NE(thenPrint, nullptr);
    auto elsePrint = std::dynamic_pointer_cast<PrintStmt>(ifStmt->elseBranch);
    ASSERT_NE(elsePrint, nullptr);
}

TEST(ParserTest, ParsesIfWithoutElse) {
    auto stmt = parseSingleStatement("if (x == 1) print x;");
    auto ifStmt = std::dynamic_pointer_cast<IfStmt>(stmt);
    ASSERT_NE(ifStmt, nullptr);
    ASSERT_EQ(ifStmt->elseBranch, nullptr);
}

TEST(ParserTest, ParsesWhileLoop) {
    auto stmt = parseSingleStatement("while (x < 10) print x;");
    auto whileStmt = std::dynamic_pointer_cast<WhileStmt>(stmt);
    ASSERT_NE(whileStmt, nullptr);
    auto condition = std::dynamic_pointer_cast<Binary>(whileStmt->condition);
    ASSERT_NE(condition, nullptr);
    EXPECT_EQ(condition->op, "<");
}

TEST(ParserTest, ParsesForLoopWithAllClauses) {
    auto stmt = parseSingleStatement("for (let i = 0; i < 10; i = i + 1) print i;");
    auto block = std::dynamic_pointer_cast<BlockStmt>(stmt);
    ASSERT_NE(block, nullptr);
    ASSERT_EQ(block->statements.size(), 2);
    auto varStmt = std::dynamic_pointer_cast<VarStmt>(block->statements[0]);
    ASSERT_NE(varStmt, nullptr);
    auto whileStmt = std::dynamic_pointer_cast<WhileStmt>(block->statements[1]);
    ASSERT_NE(whileStmt, nullptr);
}

TEST(ParserTest, ParsesForLoopWithEmptyClauses) {
    auto stmt = parseSingleStatement("for (;;) print 1;");
    auto whileStmt = std::dynamic_pointer_cast<WhileStmt>(stmt);
    ASSERT_NE(whileStmt, nullptr);
    auto literal = std::dynamic_pointer_cast<Literal>(whileStmt->condition);
    ASSERT_NE(literal, nullptr);
    EXPECT_EQ(std::get<double>(literal->value), 1.0);
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

TEST(ParserTest, ParsesReturnStatement) {
    auto stmt = parseSingleStatement("return 123;");
    auto retStmt = std::dynamic_pointer_cast<ReturnStmt>(stmt);
    ASSERT_NE(retStmt, nullptr);
    auto literal = std::dynamic_pointer_cast<Literal>(retStmt->value);
    ASSERT_NE(literal, nullptr);
    EXPECT_EQ(std::get<double>(literal->value), 123.0);
}

TEST(ParserTest, ParsesAssignmentExpression) {
    auto stmt = parseSingleStatement("x = 5;");
    auto exprStmt = std::dynamic_pointer_cast<ExpressionStmt>(stmt);
    ASSERT_NE(exprStmt, nullptr);
    auto assign = std::dynamic_pointer_cast<Assign>(exprStmt->expression);
    ASSERT_NE(assign, nullptr);
    EXPECT_EQ(assign->name, "x");
}

TEST(ParserTest, ParsesLogicalOrAndAnd) {
    auto stmt = parseSingleStatement("print a or b and c;");
    auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt);
    ASSERT_NE(printStmt, nullptr);
    auto orExpr = std::dynamic_pointer_cast<Binary>(printStmt->expression);
    ASSERT_NE(orExpr, nullptr);
}

TEST(ParserTest, ParsesEqualityAndComparison) {
    auto stmt = parseSingleStatement("print x == 1 != 2 < 3;");
    auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt);
    ASSERT_NE(printStmt, nullptr);
    auto binary = std::dynamic_pointer_cast<Binary>(printStmt->expression);
    ASSERT_NE(binary, nullptr);
}

TEST(ParserTest, ParsesArithmeticExpressions) {
    auto stmt = parseSingleStatement("print 1 + 2 * 3 - 4 / 2;");
    auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt);
    ASSERT_NE(printStmt, nullptr);
    auto binary = std::dynamic_pointer_cast<Binary>(printStmt->expression);
    ASSERT_NE(binary, nullptr);
}

TEST(ParserTest, ParsesUnaryExpressions) {
    auto stmt = parseSingleStatement("print -x;");
    auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt);
    ASSERT_NE(printStmt, nullptr);
    auto unary = std::dynamic_pointer_cast<Unary>(printStmt->expression);
    ASSERT_NE(unary, nullptr);
}

TEST(ParserTest, ParsesPostfixIncrement) {
    auto stmt = parseSingleStatement("print x++;");
    auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt);
    ASSERT_NE(printStmt, nullptr);
    auto postfix = std::dynamic_pointer_cast<Postfix>(printStmt->expression);
    ASSERT_NE(postfix, nullptr);
}

TEST(ParserTest, ParsesPostfixDecrement) {
    auto stmt = parseSingleStatement("print y--;"); 
    auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt);
    ASSERT_NE(printStmt, nullptr);
    auto postfix = std::dynamic_pointer_cast<Postfix>(printStmt->expression);
    ASSERT_NE(postfix, nullptr);
}

TEST(ParserTest, ParsesLiteralTrueFalse) {
    auto stmt = parseSingleStatement("print true;");
    auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt);
    ASSERT_NE(printStmt, nullptr);
    auto literal = std::dynamic_pointer_cast<Literal>(printStmt->expression);
    ASSERT_NE(literal, nullptr);
    EXPECT_EQ(std::get<double>(literal->value), 1.0);

    stmt = parseSingleStatement("print false;");
    printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt);
    ASSERT_NE(printStmt, nullptr);
    literal = std::dynamic_pointer_cast<Literal>(printStmt->expression);
    ASSERT_NE(literal, nullptr);
    EXPECT_EQ(std::get<double>(literal->value), 0.0);
}

TEST(ParserTest, ParsesStringLiteral) {
    auto stmt = parseSingleStatement("print \"hello\";");
    auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt);
    ASSERT_NE(printStmt, nullptr);
    auto literal = std::dynamic_pointer_cast<Literal>(printStmt->expression);
    ASSERT_NE(literal, nullptr);
    EXPECT_EQ(std::get<std::string>(literal->value), "hello");
}

TEST(ParserTest, ParsesGrouping) {
    auto stmt = parseSingleStatement("print (1 + 2) * 3;");
    auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt);
    ASSERT_NE(printStmt, nullptr);
    auto binary = std::dynamic_pointer_cast<Binary>(printStmt->expression);
    ASSERT_NE(binary, nullptr);
}

TEST(ParserTest, ParsesFunctionCall) {
    auto stmt = parseSingleStatement("print foo(1, 2);");
    auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmt);
    ASSERT_NE(printStmt, nullptr);
    auto call = std::dynamic_pointer_cast<Call>(printStmt->expression);
    ASSERT_NE(call, nullptr);
    EXPECT_EQ(call->callee, "foo");
    ASSERT_EQ(call->arguments.size(), 2);
}

TEST(ParserTest, ParsesMultipleStatements) {
    auto stmts = parseStatements("let x = 1; print x;");
    ASSERT_EQ(stmts.size(), 2);
    auto varStmt = std::dynamic_pointer_cast<VarStmt>(stmts[0]);
    ASSERT_NE(varStmt, nullptr);
    auto printStmt = std::dynamic_pointer_cast<PrintStmt>(stmts[1]);
    ASSERT_NE(printStmt, nullptr);
}

TEST(ParserTest, ThrowsOnInvalidAssignmentTarget) {
    EXPECT_THROW(parseSingleStatement("(1 + 2) = 3;"), std::runtime_error);
}

TEST(ParserTest, ThrowsOnMissingSemicolon) {
    EXPECT_THROW(parseSingleStatement("let x = 1"), std::runtime_error);
}

TEST(ParserTest, ThrowsOnUnexpectedToken) {
    EXPECT_THROW(parseSingleStatement("let = 1;"), std::runtime_error);
}

TEST(ParserTest, ThrowsOnExpectedExpression) {
    EXPECT_THROW(parseSingleStatement("print ;"), std::runtime_error);
}
