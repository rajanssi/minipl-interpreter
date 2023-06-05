#include <memory>
#include "Scanner.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"
#include "Interpreter.h"

inline void argumentError(int argc) {
    if (argc == 1) {
        std::cout << "Pass in the source file to interpret!\n";
    } else {
        std::cout << "Too many arguments, only pass in the source file to interpret!\n";
    }
}

inline void printTokens(std::vector<Token> &tokens) {
    int line = 1;
    for (auto t: tokens) {
        if (t.line != line) {
            std::cout << '\n';
            line++;
        } else std::cout << " ";
        t.print();
    }
    std::cout << '\n';
}

int main(int argc, char *argv[]) {
    if (argc <= 1 || argc > 2) {
        argumentError(argc);
        return 1;
    }

    std::unique_ptr<Scanner> scanner;
    std::unique_ptr<Parser> parser;

    try {
        scanner = std::make_unique<Scanner>(argv[1]);
    } catch (const std::runtime_error &e) {
        std::cerr << "Error trying to create Scanner: " << e.what() << '\n';
        return 1;
    }

    SymbolTable symbolTable_;
    parser = std::make_unique<Parser>(*scanner, symbolTable_);

    SemanticAnalyzer semanticAnalyzer(parser->rootNode, symbolTable_);

    scanner->scanSource();
    parser->makeAST();

    /*
    printTokens(scanner->tokens);
    parser->rootNode->print();
    symbolTable_.printSymbols();
    semanticAnalyzer.printSwitch = true;
     */

    semanticAnalyzer.beginAnalysis();

    Interpreter interpreter(parser->rootNode, symbolTable_);
    interpreter.interpret();

    std::cout << std::endl;
    std::cout << "Print final symbols...\n";
    symbolTable_.printSymbols();
    return 0;
}
