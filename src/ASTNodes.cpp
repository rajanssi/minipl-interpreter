#include "ASTNodes.h"

void ASTRoot::addStatement(ASTStatement* newStatement) {
  statementList_.push_back(newStatement);
}

ASTRoot::~ASTRoot() {
    for (auto s : statementList_) {
        delete s;
    }
}
void ASTStatement::addDeclaration(ASTDeclaration *newDeclaration) {
    declaration_ = newDeclaration;
}

void ASTDeclaration::addIdentifier(std::string &varIdent) {
    varIdent_ = varIdent;
}
void ASTDeclaration::addType(std::string &type) {
    varType_ = type;
}

void ASTDeclaration::addAssignment(ASTAssignment* assignment) {
    assignment_ = assignment;
}

void ASTAssignment::addIdentifier(std::string &varIdent) {
    varIdent_ = varIdent;
}

void ASTAssignment::addExpression(ASTExpression* expression) {
    expression_ = expression;
}
