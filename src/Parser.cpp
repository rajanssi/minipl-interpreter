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
      while (it->kind != END_LINE)
        nextToken(it);
      return statement;
    } else if (it->terminal == "print") {
      while (it->kind != END_LINE)
        nextToken(it);
      return statement;
    }
  } else if (it->kind == TokenType::IDENTIFIER) {
    // Assignment
    while (it->kind != END_LINE)
      nextToken(it);
    return statement;
  } else {
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

ASTExpression *Parser::makeExpression(TokenIterator &it) {
  return parseAddSub(it);
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
    return new ASTExpression(ASTExpression::Type::STRING, op);
  } else if (type == LEFT_PAR) {
    nextToken(it);
    auto expr = parseAddSub(it);
    match(it, RIGHT_PAR);
    nextToken(it);
    return expr;
  }
  // TODO: Sane error checking
  std::cerr << "Unexpected token, terminating\n";
  std::abort();
}

bool Parser::match(TokenIterator &it, TokenType type) {
  if (it->kind == type) {
    return true;
  }

  // TODO: Some sane error handling here
  std::cerr << "Error on line " << it->line;
  std::cerr << "Expected " << type << " and got " << it->kind << "\n";
  std::abort();
}

TokenType Parser::peek(TokenIterator &it) { return (it + 1)->kind; }

void Parser::nextToken(TokenIterator &it) { it++; }
