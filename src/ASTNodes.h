#pragma once
#include <vector>

class ASTStatement;

class ASTNode {
public:
private:

};

class ASTRoot : public ASTNode {
public:
  void addStatement(ASTStatement newStatement);
private:
  std::vector<ASTStatement> statementList_;
};

class ASTStatement : public ASTNode {
public:
private:
};

class ASTExpression : public ASTNode {
public:
private:
};
