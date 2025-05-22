#ifndef PARSER_HPP
#define PARSER_HPP

#include "token.hpp"
#include "expr.hpp"
#include "stmt.hpp"
#include <vector>
#include <memory>
#include <string>

class Parser {
public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens), current(0) {}
    std::shared_ptr<Stmt> parseStatement();
    std::vector<std::shared_ptr<Stmt>> parse();

private:
    std::shared_ptr<Expr> parseExpression();
    std::shared_ptr<Expr> parseAssignment();
    std::shared_ptr<Expr> parseEquality();
    std::shared_ptr<Expr> parseComparison();
    std::shared_ptr<Expr> parseTerm();
    std::shared_ptr<Expr> parseUnary();
    std::shared_ptr<Expr> parseFactor();
    std::shared_ptr<Expr> parsePrimary();
    std::vector<std::shared_ptr<Stmt>> parseBlock();
    std::shared_ptr<Stmt> parseFunction();
    std::shared_ptr<Stmt> parseReturn();
    std::shared_ptr<Expr> parseLogicOr();
    std::shared_ptr<Expr> parseLogicAnd();
    std::shared_ptr<Expr> parseCall();
    std::shared_ptr<Stmt> parseForStatement();
    std::shared_ptr<Expr> parsePostfix();
    bool match(const std::vector<TokenType>& types);
    bool check(TokenType type);
    Token advance();
    Token consume(TokenType type, const std::string& message);
    bool isAtEnd();
    Token peek();
    Token previous();

    const std::vector<Token>& tokens;
    size_t current;
};

#endif
