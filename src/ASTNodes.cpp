#include <iostream>

#include "ASTNodes.h"


void ASTRoot::addStatement(ASTStatement *newStatement) {
    statementList_.push_back(newStatement);
}

ASTRoot::~ASTRoot() {
    for (auto s: statementList_) {
        delete s;
    }
}

void ASTRoot::print(int indent) {
    for (auto s: statementList_) {
        s->print(0);
    }
}

void ASTStatement::print(int indent) {
    std::string indentStr(indent, ' ');
    std::cout << indentStr << "Statement: \n";
    if (declaration_) {
        declaration_->print(indent + 2);
    } else if (assignment_) {
        assignment_->print(indent + 2);
    } else if (read_) {
        read_->print(indent + 2);
    } else if (print_) {
        print_->print(indent + 2);
    } else if (if_) {
        if_->print(indent + 2);
    }
}

void ASTStatement::addDeclaration(ASTDeclaration *newDeclaration) {
    declaration_ = newDeclaration;
}

void ASTStatement::addAssignment(ASTAssignment *newAssignment) {
    assignment_ = newAssignment;
}

void ASTStatement::addRead(ASTRead *newRead) {
    read_ = newRead;
}

void ASTStatement::addPrint(ASTPrint *newPrint) {
    print_ = newPrint;
}

void ASTStatement::addIf(ASTIf *newIf) {
    if_ = newIf;
}

void ASTDeclaration::print(int indent) {
    std::string indentStr(indent, ' ');
    std::cout << indentStr << "Declaration: " << varIdent_ << " : " << varType_ << '\n';
    if (assignment_) {
        assignment_->print(indent + 2);
    }
}

void ASTDeclaration::addIdentifier(std::string &varIdent) {
    varIdent_ = varIdent;
}

void ASTDeclaration::addType(std::string &type) { varType_ = type; }

void ASTDeclaration::addAssignment(ASTAssignment *assignment) {
    assignment_ = assignment;
}

void ASTAssignment::print(int indent) {
    std::string indentStr(indent, ' ');
    std::cout << indentStr << "Assignment: " << varIdent_ << '\n';
    if (expression_) {
        indentStr = "  " + indentStr;
        std::cout << indentStr << "Expression:";
        expression_->print(indent + 2);
        std::cout << '\n';
    }
}

void ASTAssignment::addIdentifier(std::string &varIdent) {
    varIdent_ = varIdent;
}

void ASTAssignment::addExpression(ASTExpression *expression) {
    expression_ = expression;
}

void ASTExpression::print(int indent) {
    if (type == Type::NUMBER || type == Type::IDENTIFIER || type == Type::STRING || type == Type::BOOL) {
        std::cout << " " << value;
    } else if (type == Type::NOT) {
        std::cout << " ( ";
        std::cout << " " << value;
        left->print(indent);
        std::cout << " )";

    } else {
        std::cout << " (";
        std::cout << " " << value;
        left->print(indent);
        right->print(indent);
        std::cout << " )";
    }

}

void ASTRead::addIdentifier(std::string &varIdent) {
    varIdent_ = varIdent;
}

void ASTRead::print(int indent) {
    std::string indentStr(indent, ' ');
    std::cout << indentStr << "Read: " << varIdent_ << '\n';
}

void ASTPrint::addExpression(ASTExpression *expression) {
    expression_ = expression;
}

void ASTPrint::print(int indent) {
    std::string indentStr(indent, ' ');
    std::cout << indentStr << "Print:";
    expression_->print(indent);
    std::cout << std::endl;
}

void ASTIf::addCondition(ASTExpression *newCondition) {
    condition_ = newCondition;
}

void ASTIf::addStatement(ASTStatement *newStatement, bool branch) {
    if (branch)
        elseStatementList_.push_back(newStatement);
    else
        statementList_.push_back(newStatement);

}

void ASTIf::print(int indent) {
    std::string indentStr(indent, ' ');
    std::cout << indentStr << "If: ";
    if (condition_) {
        condition_->print(indent + 2);
    }
    std::cout << '\n';
    for (auto& s : statementList_) {
        s->print(indent + 2);
    }
}

void ASTLoop::print(int indent) {

}

int ASTLoop::increment() {
    current_++;
    return current_;
}