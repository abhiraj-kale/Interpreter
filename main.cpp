#include "scanner.hpp"
#include "parser.hpp"
#include "interpreter.hpp"

#include <iostream>
#include <string>

int main() {
    Interpreter interpreter;

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        Scanner scanner(line);
        std::vector<Token> tokens = scanner.scanTokens();

        Parser parser(tokens);
        ExprPtr expression = parser.parseExpr();
        
        interpreter.interpret({ expression });
    }

    return 0;
}
