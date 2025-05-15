#include "scanner.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "stmt.hpp"
#include <iostream>
#include <string>

void printInstructions() {
    std::cout << "========================================\n";
    std::cout << "       C++ Expression Interpreter       \n";
    std::cout << "========================================\n";
    std::cout << "Features:\n";
    std::cout << "  - Arithmetic: +, -, *, /\n";
    std::cout << "  - Comparison: ==, !=, >, >=, <, <=\n";
    std::cout << "  - Strings and variable support\n";
    std::cout << "  - Variable declarations: var/let <name> = <value>;\n";
    std::cout << "  - Output via: print <expression>;\n";
    std::cout << "\nExamples:\n";
    std::cout << "  var x = 10;\n";
    std::cout << "  let name = \"Alice\";\n";
    std::cout << "  print x + 5;\n";
    std::cout << "  print name;\n";
    std::cout << "\nPress Ctrl+C or submit an empty line to exit.\n";
    std::cout << "========================================\n\n";
}

int main() {
    Interpreter interpreter;
    printInstructions();

    std::string line;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line) || line.empty()) break;

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

    std::cout << "Goodbye.\n";
    return 0;
}
