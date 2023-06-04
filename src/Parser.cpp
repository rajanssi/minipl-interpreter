#include "Parser.h"
#include "ASTNodes.h"
#include "Scanner.h"
#include <cstdlib>

void Parser::makeAST() {
    rootNode = std::make_unique<ASTRoot>();

    for (auto it = scanner_.tokens.begin(); it < scanner_.tokens.end(); it++) {
        rootNode->addStatement(makeStatement(it));
    }
}

ASTStatement *Parser::makeStatement(TokenIterator &it) {
    auto *statement = new ASTStatement;

    if (it->kind == TokenType::KEYWORD) {
        if (it->terminal == "var") {
            nextToken(it);
            statement->addDeclaration(makeDeclaration(it));
        } else if (it->terminal == "read") {
            nextToken(it);
            statement->addRead(makeRead(it));
        } else if (it->terminal == "print") {
            nextToken(it);
            statement->addPrint(makePrint(it));
        } else if (it->terminal == "for") {
            while (it->kind != END_LINE)
                nextToken(it);
            return statement;
        } else if (it->terminal == "if") {
            while (it->kind != END_LINE)
                nextToken(it);
            return statement;
        }
    } else if (it->kind == TokenType::IDENTIFIER) {
        // Assignment
        statement->addAssignment(makeAssignment(it));
        return statement;
    } else {
        // TODO: Shouldn't go here, so error handling
        while (it->kind != END_LINE)
            nextToken(it);
        return statement;
    }

    return statement;
}

ASTDeclaration *Parser::makeDeclaration(TokenIterator &it) {
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


    if (type == "int") {
        symbolTable_.addSymbol(identifier, new Symbol{SymbolType::INT, identifier, 0});
    } else if (type == "string") {
        symbolTable_.addSymbol(identifier, new Symbol{SymbolType::STRING, identifier, std::string("")});
    } else if (type == "bool") {
        symbolTable_.addSymbol(identifier, new Symbol{SymbolType::BOOL, identifier, false});
    } else {
        // TODO: sane error handling here
        std::cerr << "Incorrect symbol type :" << type << '\n';
        std::abort();
    }

    if (peek(it) == TokenType::ASSIGNMENT) {
        auto *assignment = new ASTAssignment;
        assignment->addIdentifier(identifier);
        nextToken(it);
        nextToken(it);
        assignment->addExpression(makeExpression(it));
        declaration->addAssignment(assignment);
    } else {
        nextToken(it);
    }
    match(it, TokenType::END_LINE);
    return declaration;
}

ASTAssignment *Parser::makeAssignment(TokenIterator &it) {
    auto *assignment = new ASTAssignment;
    match(it, IDENTIFIER);
    assignment->addIdentifier(const_cast<std::string &>(it->terminal));
    nextToken(it);
    match(it, ASSIGNMENT);
    nextToken(it);
    assignment->addExpression(makeExpression(it));
    match(it, END_LINE);
    return assignment;
}

ASTRead *Parser::makeRead(TokenIterator &it) {
    auto read = new ASTRead();
    match(it, TokenType::IDENTIFIER);
    read->addIdentifier(const_cast<std::string &>(it->terminal));
    nextToken(it);
    match(it, TokenType::END_LINE);
    return read;
}

ASTPrint *Parser::makePrint(TokenIterator &it) {
    auto print = new ASTPrint();
    print->addExpression(makeExpression(it));
    match(it, TokenType::END_LINE);
    return print;
}

ASTExpression *Parser::makeExpression(TokenIterator &it) {
    return parseLogic(it);
}

ASTExpression *Parser::parseLogic(TokenIterator &it) {
    auto leftOperand = parseAddSub(it);

    while (it->terminal == "=" || it->terminal == "&" || it->terminal == "<") {
        auto op = it->terminal;
        nextToken(it);

        auto rightOperand = parseAddSub(it);

        ASTExpression::Type type = leftOperand->type;
        SymbolType t;
        if (type == ASTExpression::Type::IDENTIFIER) {
            t = symbolTable_.getSymbol(leftOperand->value).getType();
        } else if (type == ASTExpression::Type::BOOL || type == ASTExpression::Type::LESS_BOOL ||
                   type == ASTExpression::Type::EQ_BOOL || type == ASTExpression::Type::LESS_STR ||
                   type == ASTExpression::Type::EQ_STR || type == ASTExpression::Type::LESS_NUM ||
                   type == ASTExpression::Type::EQ_NUM) {
            t = SymbolType::BOOL;
        }
        if (op == "=") {
            if (t == SymbolType::INT)
                type = ASTExpression::Type::EQ_NUM;
            if (t == SymbolType::BOOL)
                type = ASTExpression::Type::EQ_BOOL;
            if (t == SymbolType::STRING)
                type = ASTExpression::Type::EQ_STR;
        } else if (op == "<") {
            if (t == SymbolType::INT)
                type = ASTExpression::Type::LESS_NUM;
            if (t == SymbolType::BOOL)
                type = ASTExpression::Type::LESS_BOOL;
            if (t == SymbolType::STRING)
                type = ASTExpression::Type::LESS_STR;
        } else {
            type = ASTExpression::Type::AND;
        }

        auto node = new ASTExpression(type, op);
        node->left = leftOperand;
        node->right = rightOperand;

        leftOperand = node;
    }

    return leftOperand;
}

ASTExpression *Parser::parseAddSub(TokenIterator &it) {
    auto leftOperand = parseMulDiv(it);

    while (it->terminal == "+" || it->terminal == "-") {
        auto op = it->terminal;
        nextToken(it);

        auto rightOperand = parseMulDiv(it);

        auto type = op == "+" ? ASTExpression::Type::ADDITION
                              : ASTExpression::Type::SUBTRACTION;
        auto node = new ASTExpression(type, op);
        node->left = leftOperand;
        node->right = rightOperand;

        leftOperand = node;
    }

    return leftOperand;
}

ASTExpression *Parser::parseMulDiv(TokenIterator &it) {
    auto leftOperand = parsePrimary(it);

    while (it->terminal == "*" || it->terminal == "/") {
        auto op = it->terminal;
        nextToken(it);

        auto rightOperand = parsePrimary(it);

        auto type = op == "*" ? ASTExpression::Type::MULTIPLICATION
                              : ASTExpression::Type::DIVISION;
        auto node = new ASTExpression(type, op);
        node->left = leftOperand;
        node->right = rightOperand;

        leftOperand = node;
    }

    return leftOperand;
}

ASTExpression *Parser::parsePrimary(TokenIterator &it) {
    auto type = it->kind;
    auto op = it->terminal;
    if (type == NUMBER) {
        nextToken(it);
        return new ASTExpression(ASTExpression::Type::NUMBER, op);
    } else if (type == STRING) {
        nextToken(it);
        return new ASTExpression(ASTExpression::Type::STRING, op);
    } else if (type == BOOLEAN) {
        nextToken(it);
        return new ASTExpression(ASTExpression::Type::BOOL, op);
    } else if (type == IDENTIFIER) {
        nextToken(it);
        return new ASTExpression(ASTExpression::Type::IDENTIFIER, op);
    } else if (type == LEFT_PAR) {
        nextToken(it);
        auto expr = parseLogic(it);
        match(it, RIGHT_PAR);
        nextToken(it);
        return expr;
    } else if (op == "!") {
        nextToken(it);
        auto leftOpnd = parseLogic(it);
        auto unary = new ASTExpression(ASTExpression::Type::NOT, op);
        unary->left = leftOpnd;
        return unary;
    }
    // TODO: Sane error checking
    std::cerr << "Parsing error: Unexpected token, terminating\n";
    std::abort();
}

bool Parser::match(TokenIterator &it, TokenType type) {
    if (it->kind == type) {
        return true;
    }

    // TODO: Some sane error handling here
    std::cerr << "Parser error on line " << it->line << '\n';
    std::cerr << "Expected " << type << " and got " << it->kind << "\n";
    std::abort();
}

TokenType Parser::peek(TokenIterator &it) { return (it + 1)->kind; }

void Parser::nextToken(TokenIterator &it) { it++; }
