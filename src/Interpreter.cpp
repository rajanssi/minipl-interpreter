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
        auto id = statement->assignment_->varIdent_;
        auto type = symbolTable_.getSymbol(id).getType();
        switch (type) {
            case SymbolType::INT:
                interpretAssignment<int>(statement->assignment_);
                return;
            case SymbolType::STRING:
                interpretAssignment<std::string>(statement->assignment_);
                return;
            case SymbolType::BOOL:
                interpretAssignment<bool>(statement->assignment_);
                return;
            case SymbolType::UNDEFINED:
                // TODO: Error handling
                std::cerr << "Cannot interpret undefined symbol\n";
                std::abort();
        }

    } else if (statement->print_) {
        interpretPrint(statement->print_);
    } else if (statement->read_) {
        interpretRead(statement->read_);
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
        T val;
        val = interpretExpression<T>(assignment->expression_);
        symbolTable_.setSymbolValue(assignment->varIdent_, val);
        return val;
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
        case ASTExpression::Type::IDENTIFIER:
            return symbolTable_.getSymbol(expression->value).getIntValue();
    }

    // TODO: error handling if it gets here
    return 0xdeadbeef;
}

template<>
std::string Interpreter::interpretExpression(ASTExpression *expression) {
    switch (expression->type) {
        case ASTExpression::Type::STRING:
            return expression->value;
        case ASTExpression::Type::IDENTIFIER:
            return symbolTable_.getSymbol(expression->value).getStringValue();
        case ASTExpression::Type::ADDITION: {
            auto lVal = interpretExpression<std::string>(expression->left);
            auto rVal = interpretExpression<std::string>(expression->right);
            return lVal + rVal;
        }
        case ASTExpression::Type::EQ_STR: {
            break;
        }
    }
    std::cerr << "Error interpreting string\n";
    std::abort();
}

template<>
bool Interpreter::interpretExpression(ASTExpression *expression) {
    switch (expression->type) {
        case ASTExpression::Type::NOT: {
            bool val = interpretExpression<bool>(expression->left);
            return !val;
        }
        case ASTExpression::Type::AND: {
            bool lVal = interpretExpression<bool>(expression->left);
            bool rVal = interpretExpression<bool>(expression->right);
            return lVal && rVal;
        }
        case ASTExpression::Type::EQ_NUM: {
            int lVal = interpretExpression<int>(expression->left);
            int rVal = interpretExpression<int>(expression->right);
            return lVal == rVal;
        }
        case ASTExpression::Type::LESS_NUM: {
            int lVal = interpretExpression<int>(expression->left);
            int rVal = interpretExpression<int>(expression->right);
            return lVal < rVal;
        }
        case ASTExpression::Type::EQ_STR: {
            auto lVal = interpretExpression<std::string>(expression->left);
            auto rVal = interpretExpression<std::string>(expression->right);
            return lVal == rVal;
        }
        case ASTExpression::Type::LESS_STR: {
            auto lVal = interpretExpression<std::string>(expression->left);
            auto rVal = interpretExpression<std::string>(expression->right);
            return lVal.length() < rVal.length();
        }
        case ASTExpression::Type::EQ_BOOL: {
            bool lVal = interpretExpression<bool>(expression->left);
            bool rVal = interpretExpression<bool>(expression->right);
            return lVal == rVal;
        }
        case ASTExpression::Type::LESS_BOOL: {
            bool lVal = interpretExpression<bool>(expression->left);
            bool rVal = interpretExpression<bool>(expression->right);
            return lVal < rVal;
        }
        case ASTExpression::Type::BOOL:
            return expression->value == "true";
        case ASTExpression::Type::IDENTIFIER:
            return symbolTable_.getSymbol(expression->value).getBoolValue();
    }

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
        case ASTExpression::Type::EQ_NUM:
        case ASTExpression::Type::LESS_NUM:
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

void Interpreter::interpretRead(ASTRead *read) {
    auto type = symbolTable_.getSymbol(read->varIdent_).getType();
    switch (type) {
        case SymbolType::INT: {
            int value;
            try {
                std::cin >> value;
            } catch (std::runtime_error e) {
                std::cerr << e.what() << '\n';
                std::abort();
            }
            symbolTable_.setSymbolValue(read->varIdent_, value);
            return;
        }
        case SymbolType::STRING: {
            std::string value;
            try {
                std::cin >> value;
            } catch (std::runtime_error e) {
                std::cerr << e.what() << '\n';
                std::abort();
            }
            symbolTable_.setSymbolValue(read->varIdent_, value);
            return;
        }
        case SymbolType::BOOL: {
            bool value;
            try {
                std::cin >> value;
            } catch (std::runtime_error e) {
                std::cerr << e.what() << '\n';
                std::abort();
            }
            symbolTable_.setSymbolValue(read->varIdent_, value);
            return;
        }
    }
}
