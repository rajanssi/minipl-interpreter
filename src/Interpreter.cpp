#include <iostream>
#include "Interpreter.h"

void Interpreter::interpret() {
    for (auto s: astRoot_->statementList_) {
        interpretStatement(s);
    }
}

void Interpreter::interpretStatement(ASTStatement *statement) {
    if (statement->declaration_) {
        interpretDeclaration(statement->declaration_);
    } else if (statement->assignment_) {
        interpretAssignment<int>(statement->assignment_);
    }

}

void Interpreter::interpretDeclaration(ASTDeclaration *declaration) {
    auto id = declaration->varIdent_;
    if (declaration->assignment_) {
        auto type = symbolTable_.getSymbol(id).getType();
        switch (type) {
            case SymbolType::INT:
                interpretAssignment<int>(declaration->assignment_);
                break;
            case SymbolType::STRING:
                symbolTable_.setSymbolValue(id, interpretAssignment<std::string>(declaration->assignment_));
                break;
            case SymbolType::BOOL:
                symbolTable_.setSymbolValue(id, interpretAssignment<bool>(declaration->assignment_));
                break;
            case SymbolType::UNDEFINED:
                break;
        }
    }
}

template<typename T>
T Interpreter::interpretAssignment(ASTAssignment *assignment) {
    if (assignment->expression_) {
        auto val = interpretExpression<int>(assignment->expression_);
        std::cout << "Assignment value: " << val << '\n';
        symbolTable_.setSymbolValue(assignment->varIdent_, val);
    }
}

template<>
int Interpreter::interpretExpression(ASTExpression *expression) {
    switch (expression->type) {
        case ASTExpression::Type::ADDITION: {
            int lVal = interpretExpression<int>(expression->left);
            int rVal = interpretExpression<int>(expression->right);
            return lVal + rVal;
        }
        case ASTExpression::Type::SUBTRACTION: {
            int lVal = interpretExpression<int>(expression->left);
            int rVal = interpretExpression<int>(expression->right);
            return lVal - rVal;
        }
        case ASTExpression::Type::MULTIPLICATION: {
            int lVal = interpretExpression<int>(expression->left);
            int rVal = interpretExpression<int>(expression->right);
            return lVal * rVal;
        }
        case ASTExpression::Type::DIVISION: {
            int lVal = interpretExpression<int>(expression->left);
            int rVal = interpretExpression<int>(expression->right);
            return lVal / rVal;
        }
        case ASTExpression::Type::NUMBER:
            return std::stoi(expression->value);
        case ASTExpression::Type::NOT:
            // TODO: Not implemented yet
            break;
        case ASTExpression::Type::AND:
            // TODO: Not implemented yet
            break;
        case ASTExpression::Type::EQ:
            // TODO: Not implemented yet
            break;
        case ASTExpression::Type::LESS:
            // TODO: Not implemented yet
            break;
        case ASTExpression::Type::STRING:
            // TODO: Not implemented yet
            break;
        case ASTExpression::Type::BOOL:
            // TODO: Not implemented yet
            break;
        case ASTExpression::Type::IDENTIFIER:
            return symbolTable_.getSymbol(expression->value).getIntValue();
    }

    return 0xdeadbeef;
}
