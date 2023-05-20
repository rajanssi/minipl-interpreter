#pragma once

#include <vector>
#include <memory>
#include <string>

class ASTStatement;
class ASTDeclaration;
class ASTExpression;
class ASTAssignment;

class ASTNode {
public:
private:

};

class ASTRoot : public ASTNode {
public:
  void addStatement(ASTStatement* newStatement);
  ~ASTRoot();
private:
  std::vector<ASTStatement*> statementList_;
};

class ASTStatement : public ASTNode {
public:
    void addDeclaration(ASTDeclaration* newDeclaration);
private:
    ASTDeclaration* declaration_ = nullptr;

};

class ASTExpression : public ASTNode {
public:
private:
    std::string varIdent_;
};

class ASTDeclaration : public ASTNode {
public:
    void addIdentifier(std::string& varIdent);
    void addType(std::string &type);
    void addAssignment(ASTAssignment* assignment);
private:
    std::string varIdent_;
    std::string varType_;
    ASTAssignment* assignment_;
};

class ASTAssignment : public ASTNode {
public:
    void addIdentifier(std::string& varIdent);
    void addExpression(ASTExpression* expression);
private:
    std::string varIdent_;

    ASTExpression* expression_;
};