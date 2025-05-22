#include "scanner.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "stmt.hpp"
#include <iostream>
#include <string>
#include "repl.hpp"

void printInstructions(std::ostream& out) {
    out << "========================================\n";
    out << "       C++ Expression Interpreter       \n";
    out << "========================================\n";
    out << "Features:\n";
    out << "  - Arithmetic: +, -, *, /\n";
    out << "  - Comparison: ==, !=, >, >=, <, <=\n";
    out << "  - Strings and variable support\n";
    out << "  - Variable declarations: var/let <name> = <value>;\n";
    out << "  - Output via: print <expression>;\n";
    out << "\nExamples:\n";
    out << "  var x = 10;\n";
    out << "  let name = \"Alice\";\n";
    out << "  print x + 5;\n";
    out << "  print name;\n";
    out << "\nPress Ctrl+C or submit an empty line to exit.\n";
    out << "========================================\n\n";
}

int runRepl(std::istream& in, std::ostream& out) {
    Interpreter interpreter;
    printInstructions(out);

    std::string line;
    while (true) {
        out << ">";
        if (!std::getline(in, line) || line.empty()) break;
        int openBraces = 0, closeBraces = 0;
        for (char c : line) {
            if (c == '{') openBraces++;
            else if (c == '}') closeBraces++;
        }
        while (openBraces > closeBraces) {
            std::string nextLine;
            out << "| ";
            if (!std::getline(in, nextLine)) break;
            line += "\n" + nextLine;
            for (char c : nextLine) {
                if (c == '{') openBraces++;
                else if (c == '}') closeBraces++;
            }
        }
        try {
            Scanner scanner(line);
            std::vector<Token> tokens = scanner.scanTokens();
            Parser parser(tokens);
            std::shared_ptr<Stmt> stmt = parser.parseStatement();
            interpreter.interpret({stmt});
        } catch (const std::exception& e) {
            out << "Error: " << e.what() << "\n";
        }
    }
    out << "Goodbye.\n";
    return 0;
}

int main() {
    return runRepl();
}
