#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <map>

enum TokenType {
  STRING,
  NUMBER,
  OPERATOR,
  END_LINE,
  LEFT_PAR,
  RIGHT_PAR,

  DOTDOT,
  COLON,
  ASSIGNMENT,
  TYPE_DECLARATION,
  KEYWORD,
  IDENTIFIER,

  UNKNOWN,
};

enum CharType {
  STRING_LITERAL,
  OPERATORs,
  IDENTIFIERs
};

struct Token {
  Token(TokenType kind, const std::string lexeme, const int line)
      : kind{kind}, lexeme{lexeme}, line{line} {}
  const std::string lexeme;
  const TokenType kind;
  const int line;

  void print() { std::cout << "{" << kind << ", " << lexeme << ", " << line << "}"; }
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
  void scanLineComment();
  void scanComment();
  void scanString();
  void scanNumber();

  void addToken(TokenType kind, const std::string& value);

  // constexpr int charType(const char character);

  const char getNextChar();
  const char peek();
};
