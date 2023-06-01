#pragma once

#include <memory>

#include "ASTNodes.h"
#include "SymbolTable.h"

class Interpreter {
public:
    Interpreter(std::unique_ptr<ASTRoot> &astRoot, SymbolTable &symbolTable) : astRoot_(astRoot),
                                                                               symbolTable_(symbolTable) {};

    void interpret();
private:
    std::unique_ptr<ASTRoot> &astRoot_;
    SymbolTable &symbolTable_;

};
