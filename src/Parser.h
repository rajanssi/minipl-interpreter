#pragma once

#include <memory>
#include <vector>

#include "Scanner.h"
#include "ASTNodes.h"

using TokenIterator = std::vector<Token>::iterator;

class Parser {
public:
    Parser(Scanner &scanner) : scanner_{scanner} {
        tokenIterator_ = scanner_.tokens.begin();
    };

    void makeAST();

private:
    Scanner &scanner_;
    std::unique_ptr<ASTRoot> rootNode_;

    std::vector<Token>::iterator tokenIterator_;

    ASTStatement* makeStatement(TokenIterator  &it);
    ASTDeclaration* makeDeclaration(TokenIterator &it);
    ASTAssignment* makeAssignment(TokenIterator &it);
    ASTExpression* makeExpression(TokenIterator &it);


    bool match(TokenIterator &it, TokenType type);
    TokenType peek(TokenIterator &it);
    void nextToken(TokenIterator &it);

};
