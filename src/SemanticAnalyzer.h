#pragma once

#include <memory>

#include "ASTNodes.h"
#include "SymbolTable.h"

class SemanticAnalyzer {
public:
    SemanticAnalyzer(std::unique_ptr<ASTRoot> &astRoot) : astRoot_(std::move(astRoot)) {};

private:
    std::unique_ptr<ASTRoot> astRoot_;
    SymbolTable symbolTable_;

};


