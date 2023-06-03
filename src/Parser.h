#pragma once

#include <memory>
#include <vector>

#include "Scanner.h"
#include "ASTNodes.h"
#include "SymbolTable.h"

using TokenIterator = std::vector<Token>::iterator;

class Parser {
public:
    Parser(Scanner &scanner, SymbolTable& symbolTable) : scanner_{scanner}, symbolTable_(symbolTable){
        tokenIterator_ = scanner_.tokens.begin();
    };

    void makeAST();
    std::unique_ptr<ASTRoot> rootNode;
private:
    Scanner &scanner_;

    std::vector<Token>::iterator tokenIterator_;

    ASTStatement* makeStatement(TokenIterator  &it);
    ASTDeclaration* makeDeclaration(TokenIterator &it);
    ASTAssignment* makeAssignment(TokenIterator &it);
    ASTExpression* makeExpression(TokenIterator &it);
    ASTRead* makeRead(TokenIterator &it);
    ASTPrint* makePrint(TokenIterator &it);

    ASTExpression *parseLogic(TokenIterator &it);
    ASTExpression* parseAddSub(TokenIterator &it);
    ASTExpression* parseMulDiv(TokenIterator &it);
    ASTExpression* parsePrimary(TokenIterator &it);


    bool match(TokenIterator &it, TokenType type);
    TokenType peek(TokenIterator &it);
    void nextToken(TokenIterator &it);

    SymbolTable& symbolTable_;

};
