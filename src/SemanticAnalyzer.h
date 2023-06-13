#pragma once

#include <memory>

#include "ASTNodes.h"
#include "SymbolTable.h"

class SemanticAnalyzer {
public:
    SemanticAnalyzer(std::unique_ptr<ASTRoot> &astRoot, SymbolTable &symbolTable) : astRoot_(astRoot),
                                                                                    symbolTable_(symbolTable) {};

    void beginAnalysis();

    bool printSwitch = false;
private:
    std::unique_ptr<ASTRoot> &astRoot_;
    SymbolTable &symbolTable_;

    void checkStatement(ASTStatement* statement);
    void checkDeclaration(ASTDeclaration *declaration);
    void checkAssignment(ASTAssignment *assignment);
    SymbolType checkExpression(ASTExpression *expression);
    void checkIf(ASTIf *conditional);
    void checkPrint(ASTPrint *print);
    void checkRead(ASTRead *read);
    void checkLoop(ASTLoop* loop);

    std::string typePrinter(SymbolType type);
};


