#include <iostream>
#include "SemanticAnalyzer.h"

void SemanticAnalyzer::beginAnalysis() {
    for (auto s: astRoot_->statementList_) {
        if (s->declaration_) {
            checkDeclaration(s->declaration_);
        } else if (s->assignment_) {
            checkAssignment(s->assignment_);
        } else if (s->print_) {
            checkPrint(s->print_);
        } else if (s->read_) {
            checkRead(s->read_);
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


void SemanticAnalyzer::checkPrint(ASTPrint *print) {
    auto type = checkExpression(print->expression_);
    if (type == SymbolType::BOOL) {
        std::cerr << "Semantic error: can't print out boolean values\n";
        std::abort();
    }
}

void SemanticAnalyzer::checkRead(ASTRead *read) {
    auto id = read->varIdent_;
    symbolTable_.getSymbol(id);
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
        if (expression->type == Type::EQ || expression->type == Type::LESS || expression->type == Type::AND) {

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
