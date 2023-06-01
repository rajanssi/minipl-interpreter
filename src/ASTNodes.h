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

struct ASTRoot {
    void addStatement(ASTStatement *newStatement);

    void print(int indent = 0);

    ~ASTRoot();

    std::vector<ASTStatement *> statementList_;
};

struct ASTStatement {
    void addDeclaration(ASTDeclaration *newDeclaration);

    void addAssignment(ASTAssignment *newAssignment);

    void print(int indent = 0);

    ASTDeclaration *declaration_ = nullptr;
    ASTAssignment *assignment_ = nullptr;
};

struct ASTExpression {
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
        BOOL,
        IDENTIFIER
    } type;

    std::string value;
    ASTExpression *left = nullptr;
    ASTExpression *right = nullptr;

    ASTExpression(Type type, std::string value)
            : type{type}, value(std::move(value)) {}


    void print(int indent = 0);
};

struct ASTDeclaration {
    void addIdentifier(std::string &varIdent);
    void addType(std::string &type);
    void addAssignment(ASTAssignment *assignment);
    void print(int indent = 0);

    std::string varIdent_;
    std::string varType_;
    ASTAssignment *assignment_;
};

struct ASTAssignment {
    void addIdentifier(std::string &varIdent);
    void addExpression(ASTExpression *expression);
    void print(int indent = 0);

    std::string varIdent_;
    ASTExpression *expression_;
};
