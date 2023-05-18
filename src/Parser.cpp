#include "Parser.h"
#include "ASTNodes.h"

void Parser::makeAST() {
  for (auto it = scanner_.tokens.begin(); it < scanner_.tokens.end(); it++) {
    ASTStatement s;
    rootNode_->addStatement(s);
    makeStatement(it);
    std::cout << "\n";
  }
}

ASTStatement Parser::makeStatement(std::vector<Token>::iterator& it) {
  ASTStatement statement;
  //it->print();

  return statement;
}
