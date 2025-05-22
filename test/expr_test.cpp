#include "gtest/gtest.h"
#include "expr.hpp"
#include "stmt.hpp"
#include "token.hpp"
#include <memory>
#include <variant>
#include <string>
#include <vector>
#include <map>

struct DummyFunction : public FunctionStmt {
    DummyFunction(const std::string& name, const std::vector<std::string>& params, const std::vector<std::shared_ptr<Stmt>>& body)
        : FunctionStmt(name, params, body) {}
};

TEST(CallExprTest, CallsFunctionAndReturnsValue) {
    auto retStmt = std::make_shared<ReturnStmt>(std::make_shared<Literal>(42.0));
    auto fn = std::make_shared<DummyFunction>("foo", std::vector<std::string>{"x"}, std::vector<std::shared_ptr<Stmt>>{retStmt});

    Environment env;
    env["foo"] = std::static_pointer_cast<Stmt>(fn);

    auto arg = std::make_shared<Literal>(10.0);
    Call call("foo", std::vector<std::shared_ptr<Expr>>{arg});

    Value result = call.evaluate(env);
    ASSERT_TRUE(std::holds_alternative<double>(result));
    EXPECT_EQ(std::get<double>(result), 42.0);
}

TEST(CallExprTest, ThrowsOnUndefinedFunction) {
    Environment env;
    Call call("bar", {});
    EXPECT_THROW(call.evaluate(env), std::runtime_error);
}

TEST(CallExprTest, ThrowsIfNotFunction) {
    Environment env;
    env["foo"] = 123.0;
    Call call("foo", {});
    EXPECT_THROW(call.evaluate(env), std::runtime_error);
}

TEST(PostfixExprTest, IncrementsVariable) {
    Environment env;
    env["x"] = 5.0;
    auto var = std::make_shared<Variable>("x");
    Token incToken(TokenType::INCREMENT, "++", 0, 0);
    Postfix postfix(var, incToken);

    Value result = postfix.evaluate(env);
    EXPECT_EQ(std::get<double>(result), 5.0);
    EXPECT_EQ(std::get<double>(env["x"]), 6.0);
}

TEST(PostfixExprTest, DecrementsVariable) {
    Environment env;
    env["y"] = 7.0;
    auto var = std::make_shared<Variable>("y");
    Token decToken(TokenType::DECREMENT, "--", 0, 0);
    Postfix postfix(var, decToken);

    Value result = postfix.evaluate(env);
    EXPECT_EQ(std::get<double>(result), 7.0);
    EXPECT_EQ(std::get<double>(env["y"]), 6.0);
}

TEST(PostfixExprTest, ThrowsOnNonVariableOperand) {
    Environment env;
    auto lit = std::make_shared<Literal>(1.0);
    Token incToken(TokenType::INCREMENT, "++", 0, 0);
    Postfix postfix(lit, incToken);
    EXPECT_THROW(postfix.evaluate(env), std::runtime_error);
}

TEST(PostfixExprTest, ThrowsOnNonNumericVariable) {
    Environment env;
    env["z"] = std::string("not a number");
    auto var = std::make_shared<Variable>("z");
    Token incToken(TokenType::INCREMENT, "++", 0, 0);
    Postfix postfix(var, incToken);
    EXPECT_THROW(postfix.evaluate(env), std::runtime_error);
}

TEST(PostfixExprTest, ThrowsOnUnknownOperator) {
    Environment env;
    env["a"] = 1.0;
    auto var = std::make_shared<Variable>("a");
    Token unknownToken(TokenType::PLUS, "+", 0, 0);
    Postfix postfix(var, unknownToken);
    EXPECT_THROW(postfix.evaluate(env), std::runtime_error);
}
