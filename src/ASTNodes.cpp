#include "ASTNodes.h"

void ASTRoot::addStatement(ASTStatement newStatement) {
  statementList_.push_back(newStatement);
}
