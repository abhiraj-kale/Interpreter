#include <gtest/gtest.h>
#include "scanner.hpp"

TEST(ScannerTest, SingleCharacterTokens) {
    Scanner scanner("()+-*/;{},");
    auto tokens = scanner.scanTokens();
    ASSERT_EQ(tokens.size(), 11);
    EXPECT_EQ(tokens[0].type, TokenType::LEFT_PAREN);
    EXPECT_EQ(tokens[1].type, TokenType::RIGHT_PAREN);
    EXPECT_EQ(tokens[2].type, TokenType::PLUS);
    EXPECT_EQ(tokens[3].type, TokenType::MINUS);
    EXPECT_EQ(tokens[4].type, TokenType::STAR);
    EXPECT_EQ(tokens[5].type, TokenType::SLASH);
    EXPECT_EQ(tokens[6].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[7].type, TokenType::LEFT_BRACE);
    EXPECT_EQ(tokens[8].type, TokenType::RIGHT_BRACE);
    EXPECT_EQ(tokens[9].type, TokenType::COMMA);
    EXPECT_EQ(tokens[10].type, TokenType::END_OF_FILE);
}

TEST(ScannerTest, DoubleCharacterTokens) {
    Scanner scanner("== != <= >=");
    auto tokens = scanner.scanTokens();
    ASSERT_EQ(tokens.size(), 5);
    EXPECT_EQ(tokens[0].type, TokenType::EQUAL_EQUAL);
    EXPECT_EQ(tokens[1].type, TokenType::BANG_EQUAL);
    EXPECT_EQ(tokens[2].type, TokenType::LESS_EQUAL);
    EXPECT_EQ(tokens[3].type, TokenType::GREATER_EQUAL);
    EXPECT_EQ(tokens[4].type, TokenType::END_OF_FILE);
}

TEST(ScannerTest, StringLiteral) {
    Scanner scanner("\"hello\"");
    auto tokens = scanner.scanTokens();
    ASSERT_EQ(tokens.size(), 2);
    EXPECT_EQ(tokens[0].type, TokenType::STRING);
    EXPECT_EQ(std::any_cast<std::string>(tokens[0].literal), "hello");
}

TEST(ScannerTest, NumberLiteral) {
    Scanner scanner("123 45.67");
    auto tokens = scanner.scanTokens();
    ASSERT_EQ(tokens.size(), 3);
    EXPECT_EQ(tokens[0].type, TokenType::NUMBER);
    EXPECT_DOUBLE_EQ(std::any_cast<double>(tokens[0].literal), 123);
    EXPECT_EQ(tokens[1].type, TokenType::NUMBER);
    EXPECT_DOUBLE_EQ(std::any_cast<double>(tokens[1].literal), 45.67);
}

TEST(ScannerTest, IdentifiersAndKeywords) {
    Scanner scanner("let var foo");
    auto tokens = scanner.scanTokens();
    std::vector<TokenType> expected = {
        TokenType::LET, TokenType::VAR, TokenType::IDENTIFIER, TokenType::END_OF_FILE
    };
    ASSERT_EQ(tokens.size(), expected.size());
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(tokens[i].type, expected[i]);
    }
    EXPECT_EQ(std::any_cast<std::string>(tokens[2].literal), "foo");
}

TEST(ScannerTest, Whitespace) {
    Scanner scanner("let   var\tprint");
    auto tokens = scanner.scanTokens();
    ASSERT_EQ(tokens.size(), 4);
    EXPECT_EQ(tokens[0].type, TokenType::LET);
    EXPECT_EQ(tokens[1].type, TokenType::VAR);
    EXPECT_EQ(tokens[2].type, TokenType::PRINT);
    EXPECT_EQ(tokens[3].type, TokenType::END_OF_FILE);
}
