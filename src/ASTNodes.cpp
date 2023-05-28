#include <iostream>

#include "ASTNodes.h"


void ASTRoot::addStatement(ASTStatement *newStatement) {
  statementList_.push_back(newStatement);
}

ASTRoot::~ASTRoot() {
  for (auto s : statementList_) {
    delete s;
  }
}

void ASTRoot::print(int indent) {
  for (auto s : statementList_) {
    s->print(0);
  }
}

void ASTStatement::print(int indent) {
  std::cout << "Statement: \n";
  if (declaration_) {
    declaration_->print(indent + 2);
  } else if (assignment_) {
      assignment_->print(indent + 2);
  }
}

void ASTStatement::addDeclaration(ASTDeclaration *newDeclaration) {
  declaration_ = newDeclaration;
}

void ASTStatement::addAssignment(ASTAssignment *newAssignment) {
    assignment_  = newAssignment;
}

void ASTDeclaration::print(int indent) {
  std::string indentStr(indent, ' ');
  std::cout << indentStr << "Declaration: \n";
  if (assignment_) {
    assignment_->print(indent + 2);
  }
}

void ASTDeclaration::addIdentifier(std::string &varIdent) {
  varIdent_ = varIdent;
}

void ASTDeclaration::addType(std::string &type) { varType_ = type; }

void ASTDeclaration::addAssignment(ASTAssignment *assignment) {
  assignment_ = assignment;
}

void ASTAssignment::print(int indent) {
  std::string indentStr(indent, ' ');
  std::cout << indentStr << "Assignment: \n";
  if (expression_) {
    expression_->print(indent + 2);
  }
}

void ASTAssignment::addIdentifier(std::string &varIdent) {
  varIdent_ = varIdent;
}

void ASTAssignment::addExpression(ASTExpression *expression) {
  expression_ = expression;
}

void ASTExpression::print(int indent) {

}
