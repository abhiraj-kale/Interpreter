#include "parser.hpp"
#include "token.hpp"
#include <stdexcept>
#include <iostream>

std::shared_ptr<Stmt> Parser::parseStatement() {
    if (match({TokenType::LET, TokenType::VAR})) {
        Token nameToken = consume(TokenType::IDENTIFIER, "Expected variable name.");
        consume(TokenType::EQUAL, "Expected '=' after variable name.");
        auto initializer = parseExpression();
        consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
        return std::make_shared<VarStmt>(nameToken.lexeme, initializer);
    }
    if (match({TokenType::PRINT})) {
        auto value = parseExpression();
        consume(TokenType::SEMICOLON, "Expected ';' after value.");
        return std::make_shared<PrintStmt>(value);
    }
    if (match({TokenType::LEFT_BRACE})) {
        return std::make_shared<BlockStmt>(parseBlock());
    }
    if (match({TokenType::IF})) {
        consume(TokenType::LEFT_PAREN, "Expected '(' after 'if'.");
        auto condition = parseExpression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after if condition.");
        auto thenBranch = parseStatement();
        std::shared_ptr<Stmt> elseBranch = nullptr;
        if (match({TokenType::ELSE})) {
            elseBranch = parseStatement();
        }
        return std::make_shared<IfStmt>(condition, thenBranch, elseBranch);
    }
    if (match({TokenType::WHILE})) {
        consume(TokenType::LEFT_PAREN, "Expected '(' after 'while'.");
        auto condition = parseExpression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after condition.");
        auto body = parseStatement();
        return std::make_shared<WhileStmt>(condition, body);
    }
    if (match({TokenType::FOR})) return parseForStatement();
    if (match({TokenType::FUN})) return parseFunction();
    if (match({TokenType::RETURN})) return parseReturn();

    auto expr = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after expression.");
    return std::make_shared<ExpressionStmt>(expr);
}

std::shared_ptr<Stmt> Parser::parseForStatement() {
    consume(TokenType::LEFT_PAREN, "Expected '(' after 'for'.");

    std::shared_ptr<Stmt> initializer;
    if (match({TokenType::SEMICOLON})) {
        initializer = nullptr;
    } else if (match({TokenType::LET, TokenType::VAR})) {
        Token nameToken = consume(TokenType::IDENTIFIER, "Expected variable name.");
        consume(TokenType::EQUAL, "Expected '=' after variable name.");
        auto initExpr = parseExpression();
        consume(TokenType::SEMICOLON, "Expected ';' after variable declaration.");
        initializer = std::make_shared<VarStmt>(nameToken.lexeme, initExpr);
    } else {
        auto initExpr = parseExpression();
        consume(TokenType::SEMICOLON, "Expected ';' after loop initializer.");
        initializer = std::make_shared<ExpressionStmt>(initExpr);
    }

    std::shared_ptr<Expr> condition = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        condition = parseExpression();
    }
    consume(TokenType::SEMICOLON, "Expected ';' after loop condition.");

    std::shared_ptr<Expr> increment = nullptr;
    if (!check(TokenType::RIGHT_PAREN)) {
        increment = parseExpression();
    }
    consume(TokenType::RIGHT_PAREN, "Expected ')' after for clauses.");

    auto body = parseStatement();

    if (increment) {
        body = std::make_shared<BlockStmt>(std::vector<std::shared_ptr<Stmt>>{
            body,
            std::make_shared<ExpressionStmt>(increment)
        });
    }

    if (!condition) {
        condition = std::make_shared<Literal>(1.0);
    }

    body = std::make_shared<WhileStmt>(condition, body);

    if (initializer) {
        body = std::make_shared<BlockStmt>(std::vector<std::shared_ptr<Stmt>>{
            initializer,
            body
        });
    }

    return body;
}

std::shared_ptr<Expr> Parser::parsePostfix() {
    std::shared_ptr<Expr> expr = parsePrimary();

    while (match({TokenType::INCREMENT}) || match({TokenType::DECREMENT})) {
        Token op = previous();
        expr = std::make_shared<Postfix>(expr, op);
    }

    return expr;
}

std::shared_ptr<Expr> Parser::parseExpression() {
    return parseAssignment();
}

std::shared_ptr<Expr> Parser::parseAssignment() {
    auto expr = parseLogicOr();

    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        auto value = parseAssignment();

        if (auto var = std::dynamic_pointer_cast<Variable>(expr)) {
            return std::make_shared<Assign>(var->name, value);
        }

        throw std::runtime_error("Invalid assignment target.");
    }

    return expr;
}

std::shared_ptr<Expr> Parser::parseLogicOr() {
    auto expr = parseLogicAnd();
    while (match({TokenType::OR})) {
        Token op = previous();
        auto right = parseLogicAnd();
        expr = std::make_shared<Binary>(expr, op.lexeme, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::parseLogicAnd() {
    auto expr = parseEquality();
    while (match({TokenType::AND})) {
        Token op = previous();
        auto right = parseEquality();
        expr = std::make_shared<Binary>(expr, op.lexeme, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::parseEquality() {
    auto expr = parseComparison();
    while (match({TokenType::EQUAL_EQUAL, TokenType::BANG_EQUAL})) {
        Token op = previous();
        auto right = parseComparison();
        expr = std::make_shared<Binary>(expr, op.lexeme, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::parseComparison() {
    auto expr = parseTerm();
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token op = previous();
        auto right = parseTerm();
        expr = std::make_shared<Binary>(expr, op.lexeme, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::parseTerm() {
    auto expr = parseFactor();
    while (match({TokenType::PLUS, TokenType::MINUS})) {
        Token op = previous();
        auto right = parseFactor();
        expr = std::make_shared<Binary>(expr, op.lexeme, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::parseFactor() {
    auto expr = parseUnary();
    while (match({TokenType::STAR, TokenType::SLASH})) {
        Token op = previous();
        auto right = parseUnary();
        expr = std::make_shared<Binary>(expr, op.lexeme, right);
    }
    return expr;
}

std::shared_ptr<Expr> Parser::parseUnary() {
    if (match({TokenType::BANG, TokenType::MINUS, TokenType::PLUS_PLUS, TokenType::MINUS_MINUS})) {
        Token op = previous();
        auto right = parseUnary();
        return std::make_shared<Unary>(op, right);
    }
    return parsePostfix();
}

std::shared_ptr<Expr> Parser::parsePrimary() {
    if (match({TokenType::NUMBER})) {
        return std::make_shared<Literal>(std::any_cast<double>(previous().literal));
    }
    if (match({TokenType::STRING})) {
        return std::make_shared<Literal>(std::any_cast<std::string>(previous().literal));
    }
    if (match({TokenType::TRUE})) {
        return std::make_shared<Literal>(1.0);
    }
    if (match({TokenType::FALSE})) {
        return std::make_shared<Literal>(0.0);
    }
    if (match({TokenType::LEFT_PAREN})) {
        auto expr = parseExpression();
        consume(TokenType::RIGHT_PAREN, "Expect ')' after expression.");
        return expr;
    }
    if (match({TokenType::IDENTIFIER})) {
        std::string name = previous().lexeme;
        if (match({TokenType::LEFT_PAREN})) {
            std::vector<std::shared_ptr<Expr>> args;
            if (!check(TokenType::RIGHT_PAREN)) {
                do {
                    args.push_back(parseExpression());
                } while (match({TokenType::COMMA}));
            }
            consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments.");
            return std::make_shared<Call>(name, args);
        }
        return std::make_shared<Variable>(name);
    }
    throw std::runtime_error("Expected expression.");
}

std::vector<std::shared_ptr<Stmt>> Parser::parseBlock() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(parseStatement());
    }
    consume(TokenType::RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

std::vector<std::shared_ptr<Stmt>> Parser::parse() {
    std::vector<std::shared_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(parseStatement());
    }
    return statements;
}

std::shared_ptr<Stmt> Parser::parseFunction() {
    Token nameToken = consume(TokenType::IDENTIFIER, "Expected function name.");
    consume(TokenType::LEFT_PAREN, "Expected '(' after function name.");

    std::vector<std::string> params;
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            params.push_back(consume(TokenType::IDENTIFIER, "Expected parameter name.").lexeme);
        } while (match({TokenType::COMMA}));
    }

    consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters.");
    consume(TokenType::LEFT_BRACE, "Expected '{' before function body.");
    auto body = parseBlock();

    return std::make_shared<FunctionStmt>(nameToken.lexeme, params, body);
}

std::shared_ptr<Stmt> Parser::parseReturn() {
    auto value = parseExpression();
    consume(TokenType::SEMICOLON, "Expected ';' after return value.");
    return std::make_shared<ReturnStmt>(value);
}

bool Parser::match(const std::vector<TokenType>& types) {
    for (const auto& type : types) {
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    throw std::runtime_error(message);
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::END_OF_FILE;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}