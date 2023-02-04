#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <map>

enum TokenType {
  STRING,
  OPERATOR,
  END_LINE,
};

enum CharType {
  STRING_LITERAL,
  OPERATORs,
  IDENTIFIER
};

struct Token {
  Token(TokenType kind, const std::string lexeme)
      : kind{kind}, lexeme{lexeme} {}
  const std::string lexeme;
  const TokenType kind;

  void print() { std::cout << "{" << kind << ", " << lexeme << "}"; }
};

class Scanner {
public:
  std::vector<Token> tokens;

  Scanner(const std::string &fileName) {
    try {
      readSourceFile(fileName);
    } catch (const std::runtime_error &error) {
      throw error;
    }
  }

  // NOTE: temporary helpers
  void printChar(const char printable);
  void printSourceString();

  // NOTE: Should perhaps be driven by the parser?
  void scanSource();

private:
  std::string sourceString_;
  std::map<char, TokenType> charTable_;

  int tokenStart_ = 0;
  int currentPosition_ = 0;
  int line_ = 1;

  void readSourceFile(const std::string &fileName);

  void scanToken();
  void scanString();

  void addToken(TokenType kind, const std::string& value);

  // constexpr int charType(const char character);

  const char getNextChar();
};
