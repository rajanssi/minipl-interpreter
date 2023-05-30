#include <memory>
#include "Scanner.h"
#include "Parser.h"
#include "SemanticAnalyzer.h"

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

    parser = std::make_unique<Parser>(*scanner);

    SemanticAnalyzer semanticAnalyzer(parser->rootNode);

    scanner->scanSource();
    // NOTE: Print tokens for show
    printTokens(scanner->tokens);
    parser->makeAST();
    //parser->rootNode->print();

    std::string id = "idA";
    Symbol s1(SymbolType::INT, id, 3);
    //Symbol s2(SymbolType::INT, (std::string &) "idA", "hh!");
    //Symbol s3(SymbolType::INT, (std::string &) "idA", true);

    std::cout << s1.getId() << s1.getIntValue() << '\n';
    //std::cout << s2.getId() << s2.getStringValue() << '\n';
    return 0;
}
