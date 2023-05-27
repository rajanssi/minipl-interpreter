#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

class ASTStatement;
class ASTDeclaration;
class ASTExpression;
class ASTAssignment;
class ASTOperand;

class ASTNode {
public:
  void print(int indent = 0) {};
private:
};

class ASTRoot : public ASTNode {
public:
  void addStatement(ASTStatement *newStatement);
  void print(int indent = 0);
  ~ASTRoot();

private:
  std::vector<ASTStatement *> statementList_;
};

class ASTStatement : public ASTNode {
public:
  void addDeclaration(ASTDeclaration *newDeclaration);
  void print(int indent = 0);

private:
  ASTDeclaration *declaration_ = nullptr;
};

class ASTExpression : public ASTNode {
public:
  enum class Type {
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    DIVISION,
    NUMBER,
    NOT,
    AND,
    EQ,
    LESS,
    STRING,
    BOOL
  } type;

  std::string value;
  ASTExpression *left = nullptr;
  ASTExpression *right = nullptr;

  ASTExpression(Type type, std::string value)
      : type{type}, value(std::move(value)) {}


  void print(int indent = 0);
};

class ASTDeclaration : public ASTNode {
public:
  void addIdentifier(std::string &varIdent);
  void addType(std::string &type);
  void addAssignment(ASTAssignment *assignment);

  void print(int indent = 0);

private:
  std::string varIdent_;
  std::string varType_;
  ASTAssignment *assignment_;
};

class ASTAssignment : public ASTNode {
public:
  void addIdentifier(std::string &varIdent);
  void addExpression(ASTExpression *expression);

  void print(int indent = 0);
private:
  std::string varIdent_;

  ASTExpression *expression_;
};
