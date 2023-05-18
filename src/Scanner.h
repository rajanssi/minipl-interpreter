#pragma once

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

enum TokenType {
  STRING,
  NUMBER,
  BOOLEAN,
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
  END_OF_FILE,
};


struct Token {
  Token(TokenType kind, const std::string &terminal, const int line)
      : kind{kind}, terminal{terminal}, line{line} {}
  const std::string terminal;
  const TokenType kind;
  const int line;

  void print() {
    std::cout << "{" << kind << ", " << terminal << ", " << line << "}";
  }
};

class Scanner {
public:
  std::vector<Token> tokens;

  Scanner(const std::string &fileName) {
    fillIdCharacterTable();
    fillKeywordTable();
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
  std::set<char> idChars_;
  std::set<std::string> keywords_;

  int tokenStart_ = 0;
  int currentPosition_ = 0;
  int line_ = 1;

  void readSourceFile(const std::string &fileName);

  void scanToken();

  void scanComment();
  void scanString();
  void scanNumber(char starter);
  void scanIdentifier(char starter);

  void addToken(TokenType kind, const std::string &value);

  // constexpr int charType(const char character);

  const char getNextChar();
  const char peek();

  const void fillIdCharacterTable();
  const void fillKeywordTable();
};
