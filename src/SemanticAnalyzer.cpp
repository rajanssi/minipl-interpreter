#include <iostream>
#include "SemanticAnalyzer.h"

void SemanticAnalyzer::beginAnalysis() {
    for (auto s: astRoot_->statementList_) {
        checkStatement(s);
    }
}

void SemanticAnalyzer::checkStatement(ASTStatement *statement) {
    if (statement->declaration_) {
        checkDeclaration(statement->declaration_);
    } else if (statement->assignment_) {
        checkAssignment(statement->assignment_);
    } else if (statement->print_) {
        checkPrint(statement->print_);
    } else if (statement->read_) {
        checkRead(statement->read_);
    } else if (statement->if_) {
        checkIf(statement->if_);
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
        std::cerr << "Assignment must have expression\n";
        std::abort();
    }

    auto exprType = checkExpression(assignment->expression_);

    if (printSwitch) {
        std::cout << "Assignment for id : " << id << ", type " << typePrinter(type) << '\n';
        std::cout << "  Expression type: " << typePrinter(exprType) << '\n';
    }

    if (exprType != type) {
        std::cerr << "Semantic error: Cannot assign " << typePrinter(exprType) << " to variable " << id << " of type "
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
    ASTExpression::Type type = expression->type;

    switch (type) {
        case ASTExpression::Type::NUMBER:
            return SymbolType::INT;
        case ASTExpression::Type::STRING:
            return SymbolType::STRING;
        case ASTExpression::Type::BOOL:
            return SymbolType::BOOL;
        case ASTExpression::Type::IDENTIFIER:
            return symbolTable_.getSymbol(expression->value).getType();
        case ASTExpression::Type::NOT:
            checkExpression(expression->left);
            return SymbolType::BOOL;
    }

    auto lType = checkExpression(expression->left);
    auto rType = checkExpression(expression->right);

    if (lType != rType) {
        return SymbolType::UNDEFINED;
    }
    switch (type) {
        case ASTExpression::Type::AND: {
            if (lType == SymbolType::BOOL)
                return SymbolType::BOOL;
            else
                return SymbolType::UNDEFINED;
            case ASTExpression::Type::EQ_NUM:
            case ASTExpression::Type::LESS_NUM:
            case ASTExpression::Type::EQ_BOOL:
            case ASTExpression::Type::LESS_BOOL:
            case ASTExpression::Type::EQ_STR:
            case ASTExpression::Type::LESS_STR:
                return lType == rType ? SymbolType::BOOL : SymbolType::UNDEFINED;
        }
    }
}

void SemanticAnalyzer::checkIf(ASTIf *conditional) {
    auto type = checkExpression(conditional->condition_);
    if (type != SymbolType::BOOL) {
        std::cerr << "If statement condition must evaluate to bool\n";
        std::abort();
    }
    for (auto s: conditional->statementList_) {
        checkStatement(s);
    }
}

void SemanticAnalyzer::checkLoop(ASTLoop* loop) {
    if (symbolTable_.getSymbol(loop->ctrlVarId_).getType() != SymbolType::INT) {
        std::cerr << "For loop control variable must be type int\n";
        std::abort();
    }
    if ((checkExpression(loop->beginningExpr_) != SymbolType::INT) || checkExpression(loop->endingExpr_) != SymbolType::INT) {
        std::cerr << "For loop beginning and ending expressions must be type int\n";
        std::abort();
    }

    for (auto s : loop->statementList_) {
        checkStatement(s);
    }

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
