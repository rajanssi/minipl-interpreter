#include <iostream>
#include "SemanticAnalyzer.h"

void SemanticAnalyzer::beginAnalysis() {
    int x = 3;
    for (auto s: astRoot_->statementList_) {
        if (s->declaration_) {
            checkDeclaration(s->declaration_);
        } else if (s->assignment_) {
            checkAssignment(s->assignment_);
        }
    }
}

void SemanticAnalyzer::checkDeclaration(ASTDeclaration *declaration) {
    auto id = declaration->varIdent_;
    auto type = symbolTable_.getSymbol(id).getType();

    if (printSwitch) {
        std::cout << "Declaration for id : " << id << ", type " << typePrinter(type) << '\n';
    }

    if (declaration->assignment_) {
        checkAssignment(declaration->assignment_);
    }
}

void SemanticAnalyzer::checkAssignment(ASTAssignment *assignment) {
    auto id = assignment->varIdent_;
    auto type = symbolTable_.getSymbol(id).getType();

    if (!assignment->expression_) {
        // TODO: Some sane error handling here
        std::cerr << "Assignment must have expression\n";
        std::abort();
    }

    auto exprType = checkExpression(assignment->expression_);

    if (printSwitch) {
        std::cout << "Assignment for id : " << id << ", type " << typePrinter(type) << '\n';
        std::cout << "  Expression type: " << typePrinter(exprType) << '\n';
    }

    if (exprType != type) {
        // TODO: Some sane error handling here
        std::cerr << "Cannot assign " << typePrinter(exprType) << " to variable " << id << " of type "
                  << typePrinter(type) << '\n';
        std::abort();
    }
}

SymbolType SemanticAnalyzer::checkExpression(ASTExpression *expression) {
    typedef ASTExpression::Type Type;
    Type type = expression->type;

    switch (type) {
        case Type::NUMBER:
            return SymbolType::INT;
        case Type::STRING:
            return SymbolType::STRING;
        case Type::BOOL:
            return SymbolType::BOOL;
        case Type::IDENTIFIER:
            return symbolTable_.getSymbol(expression->value).getType();
        case Type::NOT:
            return SymbolType::UNDEFINED;
    }

    auto lType = checkExpression(expression->left);
    auto rType = checkExpression(expression->right);

    if (lType != rType) {
        // TODO: Checks for boolean logic
        if (expression->type == Type::EQ || expression->type == Type::LESS) {

        }
        return SymbolType::UNDEFINED;
    }

    return lType;
}

std::string SemanticAnalyzer::typePrinter(SymbolType type) {
    switch (type) {
        case SymbolType::STRING:
            return "STRING";
        case SymbolType::INT:
            return "INT";
        case SymbolType::BOOL:
            return "BOOL";
        case SymbolType::UNDEFINED:
            return "UNDEFINED";
    }

    return "ERROR";
}
