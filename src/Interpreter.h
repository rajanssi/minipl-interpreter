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

    void interpretStatement(ASTStatement *statement);

    void interpretDeclaration(ASTDeclaration *declaration);

    template<typename T>
    T interpretAssignment(ASTAssignment *assignment);

    template<typename T>
    T interpretExpression(ASTExpression *expression);

    void interpretPrint(ASTPrint *print);
    void interpretRead(ASTRead *read);
    void interpretIf(ASTIf *conditional);
};
