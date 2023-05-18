#pragma once

#include <memory>
#include <vector>

#include "Scanner.h"
#include "ASTNodes.h"

class Parser {
public:
  Parser(Scanner& scanner) : scanner_{scanner} {
    tokenIterator_ = scanner_.tokens.begin();
  } ;

  void makeAST();
private:
  Scanner& scanner_;
  std::unique_ptr<ASTRoot> rootNode_;

  std::vector<Token>::iterator tokenIterator_;

  ASTStatement makeStatement(std::vector<Token>::iterator& it);
  ASTExpression makeExpression(std::vector<Token>::iterator& it);

};
