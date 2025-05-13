#include "scanner.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "stmt.hpp"
#include <iostream>
#include <string>

int main() {
    Interpreter interpreter;

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line)) break;

        try {
            Scanner scanner(line);
            std::vector<Token> tokens = scanner.scanTokens();

            Parser parser(tokens);
            std::shared_ptr<Stmt> stmt = parser.parseStatement();  

            interpreter.interpret({ stmt });  
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    return 0;
}
