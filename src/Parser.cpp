#include "Parser.h"
#include "ASTNodes.h"

void Parser::makeAST() {
    rootNode_ = std::make_unique<ASTRoot>();

    for (auto it = scanner_.tokens.begin(); it < scanner_.tokens.end(); it++) {
        rootNode_->addStatement(makeStatement(it));
    }
}

ASTStatement* Parser::makeStatement(TokenIterator &it) {
    auto *statement = new ASTStatement;

    if (it->kind == TokenType::KEYWORD) {
        if (it->terminal == "var") {
            nextToken(it);
            statement->addDeclaration(makeDeclaration(it));
        } else if (it->terminal == "read") {

        } else if (it->terminal == "print") {

        }
    } else if (it->kind == TokenType::IDENTIFIER) {
        // Assignment
    } else {

    }

    return statement;
}

ASTDeclaration* Parser::makeDeclaration(TokenIterator &it) {
    auto declaration = new ASTDeclaration();
    match(it, TokenType::IDENTIFIER);
    std::string identifier = it->terminal;
    declaration->addIdentifier(const_cast<std::string &>(identifier));
    nextToken(it);
    match(it, TokenType::TYPE_DECLARATION);
    nextToken(it);
    match(it, TokenType::KEYWORD);
    std::string type = it->terminal;
    declaration->addType(const_cast<std::string &>(type));

    if (peek(it) == TokenType::ASSIGNMENT) {
        auto* assignment = new ASTAssignment;
        assignment->addIdentifier(identifier);
        nextToken(it);
        nextToken(it);
        assignment->addExpression(makeExpression(it));
        declaration->addAssignment(assignment);

        // Debugging code
        while (it->kind != TokenType::END_LINE) it++;
    } else {
        nextToken(it);
    }
    match(it, TokenType::END_LINE);
    return declaration;
}

ASTExpression* Parser::makeExpression(TokenIterator &it) {
    return nullptr;
}

bool Parser::match(TokenIterator &it, TokenType type) {
    if (it->kind == type) {
        return true;
    }

    // NOTE: Some sane error handling here
    std::cout << "Error on line " << it->line;
    std::cout << "Expected " <<  type << " and got " << it->kind << "\n";
    abort();
}

TokenType Parser::peek(TokenIterator &it) {
    return (it+1)->kind;
}

void Parser::nextToken(TokenIterator &it) {
    it++;
}