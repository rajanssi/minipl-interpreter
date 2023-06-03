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
    } else if (statement->print_) {
        interpretPrint(statement->print_);
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
        symbolTable_.setSymbolValue(assignment->varIdent_, val);
    }
    return 0;
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
        case ASTExpression::Type::BOOL:
            // TODO: Not implemented yet
            break;
        case ASTExpression::Type::IDENTIFIER:
            return symbolTable_.getSymbol(expression->value).getIntValue();
    }

    return 0xdeadbeef;
}

template<>
std::string Interpreter::interpretExpression(ASTExpression *expression) {
    switch (expression->type) {
        case ASTExpression::Type::STRING:
            return expression->value;
        case ASTExpression::Type::IDENTIFIER:
            return symbolTable_.getSymbol(expression->value).getStringValue();
    }
    return "Error interpreting string\n";
}

void Interpreter::interpretPrint(ASTPrint *print) {
    switch (print->expression_->type) {
        case ASTExpression::Type::ADDITION:
        case ASTExpression::Type::SUBTRACTION:
        case ASTExpression::Type::MULTIPLICATION:
        case ASTExpression::Type::DIVISION:
        case ASTExpression::Type::NUMBER: {
            auto output = interpretExpression<int>(print->expression_);
            std::cout << output;
            return;
        }
        case ASTExpression::Type::NOT:
        case ASTExpression::Type::AND:
        case ASTExpression::Type::EQ:
        case ASTExpression::Type::LESS:
        case ASTExpression::Type::BOOL: {
            std::cerr << "ERROR: Printing boolean value not allowed\n";
            std::abort();
            break;
        }
        case ASTExpression::Type::STRING: {
            auto output = interpretExpression<std::string>(print->expression_);
            std::cout << output;
            return;
        }
        case ASTExpression::Type::IDENTIFIER: {
            auto id = print->expression_->value;
            auto value = symbolTable_.getSymbol(id);
            switch (value.getType()) {
                case SymbolType::INT:
                    std::cout << value.getIntValue();
                    return;
                case SymbolType::STRING:
                    std::cout << value.getStringValue();
                    return;
                case SymbolType::BOOL:
                    std::cerr << "ERROR: Printing boolean value not allowed\n";
                    std::abort();
                    break;
                case SymbolType::UNDEFINED:
                    std::cerr << "ERROR: Undefined value for identifier " << id << '\n';
                    std::abort();
                    break;
            }
        }
    }
}
