#ifndef PARSER_HPP
#define PARSER_HPP

#include <vector>
#include <string>
#include <memory>
#include "expr.hpp"
#include "stmt.hpp"
#include "token.hpp"

// Forward declaration in case "stmt.hpp" does not define Stmt directly
class Stmt;

class Parser {
    const std::vector<Token>& tokens;
    int current = 0;

public:
    Parser(const std::vector<Token>& tokens) : tokens(tokens) {}

    std::shared_ptr<Stmt> parseStatement();
    std::shared_ptr<Expr> parseExpression();
    std::shared_ptr<Expr> parseAssignment();
    std::shared_ptr<Expr> parseEquality();
    std::shared_ptr<Expr> parseTerm();
    std::shared_ptr<Expr> parseFactor();
    std::shared_ptr<Expr> parsePrimary();

private:
    bool match(const std::vector<TokenType>& types);
    bool check(TokenType type);
    Token advance();
    Token consume(TokenType type, const std::string& message);
    bool isAtEnd();
    Token peek();
    Token previous();
};

#endif // PARSER_HPP
