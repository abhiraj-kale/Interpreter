#include "scanner.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "stmt.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
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

// Script mode: run a whole program at once, quietly (no banner, no prompts).
// Unlike the REPL, the entire source is scanned and parsed as one unit via
// Parser::parse(), so statements can span lines and blank lines are fine.
// Used by the web IDE, and for running .clang script files from the CLI.
int runScript(std::istream& in, std::ostream& out) {
    std::stringstream buffer;
    buffer << in.rdbuf();
    std::string source = buffer.str();

    try {
        Scanner scanner(source);
        std::vector<Token> tokens = scanner.scanTokens();
        Parser parser(tokens);
        std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
        Interpreter interpreter;
        interpreter.interpret(statements);
    } catch (const std::exception& e) {
        out << "Error: " << e.what() << "\n";
        return 1;
    }
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        if (std::strcmp(argv[1], "--script") == 0) {
            // program is piped in on stdin
            return runScript(std::cin, std::cout);
        }
        // otherwise treat the argument as a script file path
        std::ifstream file(argv[1]);
        if (!file) {
            std::cerr << "Error: could not open file '" << argv[1] << "'\n";
            return 1;
        }
        return runScript(file, std::cout);
    }
    return runRepl();
}
