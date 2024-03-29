#pragma once

#include <memory>
#include <string>
#include <variant>
#include <vector>

struct ASTStatement;
struct ASTDeclaration;
struct ASTExpression;
struct ASTAssignment;
struct ASTRead;
struct ASTPrint;
struct ASTIf;
struct ASTLoop;

class ASTNode {
    void print(int indent = 0) {};
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

    void addRead(ASTRead *newRead);

    void addPrint(ASTPrint *newPrint);

    void addIf(ASTIf* newIf);

    void addLoop(ASTLoop* newLoop);

    void print(int indent = 0);

    ASTDeclaration *declaration_ = nullptr;
    ASTAssignment *assignment_ = nullptr;
    ASTRead *read_ = nullptr;
    ASTPrint *print_ = nullptr;
    ASTIf* if_ = nullptr;
    ASTLoop* loop_ = nullptr;
};

struct ASTExpression {
    enum class Type {
        ADDITION,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
        NUMBER,
        NOT,
        AND,
        EQ_NUM,
        EQ_BOOL,
        EQ_STR,
        LESS_NUM,
        LESS_BOOL,
        LESS_STR,
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

struct ASTRead {
    void addIdentifier(std::string &varIdent);

    void print(int indent = 0);

    std::string varIdent_;
};

struct ASTPrint {
    void addExpression(ASTExpression *expression);

    void print(int indent = 0);

    ASTExpression *expression_;
};

struct ASTIf {
    void addCondition(ASTExpression* newCondition);
    void addStatement(ASTStatement* newStatement);
    void addElseStatement(ASTStatement* newStatement);

    void print(int indent = 0);

    bool else_ = false;
    ASTExpression* condition_ = nullptr;
    std::vector<ASTStatement*> statementList_;
    std::vector<ASTStatement*> elseStatementList_;
};

struct ASTLoop {
    void print(int indent = 0);
    void addStatement(ASTStatement* newStatement);

    int increment();

    int current_ = 0;
    int end_ = 0;

    ASTExpression* beginningExpr_;
    ASTExpression* endingExpr_;
    std::string ctrlVarId_;
    std::vector<ASTStatement*> statementList_;
};