#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

enum TokenType {
  STRING,

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
  std::vector<std::string> strings;
  std::vector<Token> tokens;

  Scanner(const std::string &fileName) {
    try {
      readSourceFile(fileName);
    } catch (const std::runtime_error &e) {
      throw e;
    }
  }

  // NOTE: temporary helpers
  void printChar(const char printable);
  void printSourceString();

  void scanSource() {
    while (currentPosition <= sourceString.length()) {
      tokenStart = currentPosition;
      scanToken();
    }
  }

private:
  std::string sourceString;

  int tokenStart = 0;
  int currentPosition = 0;
  int line = 1;

  void readSourceFile(const std::string &fileName);
  void scanToken();
  void scanString();
  void addToken(TokenType kind, const std::string value);
  const char getNextChar();
};

const char Scanner::getNextChar() { return sourceString[currentPosition++]; }

void Scanner::addToken(TokenType kind, const std::string value) {
  tokens.push_back({kind, value});
}

void Scanner::scanToken() {
  char character = getNextChar();
  switch (character) {
  case '"':
    scanString();
  default:
    break;
  }
}

void Scanner::scanString() {
  char nextChar = ' ';
  while (nextChar != '"' && currentPosition <= sourceString.length()) {
    nextChar = getNextChar();
    if (nextChar == '\\') {
      currentPosition++;
    }
  }

  addToken(TokenType::STRING, sourceString.substr(tokenStart, currentPosition - tokenStart));
}

void Scanner::printChar(const char character) {
  std::cout << character << '\r';
  std::cout.flush();
}

void Scanner::printSourceString() { std::cout << sourceString << '\n'; }

void Scanner::readSourceFile(const std::string &fileName) {
  std::ifstream file(fileName);

  if (!file) {
    throw std::runtime_error("Failed to read file " + fileName);
  }

  while (file) {
    std::string inputStream;
    std::getline(file, inputStream);
    sourceString += inputStream;
  }
}

void argumentError(int argc) {
  std::cout << "Incorrect amount of arguments!" << '\n';
}

int main(int argc, char *argv[]) {
  if (argc <= 1 || argc > 2) {
    argumentError(argc);
    return 1;
  }

  std::unique_ptr<Scanner> scanner;

  try {
    scanner = std::make_unique<Scanner>(argv[1]);
  } catch (const std::runtime_error &e) {
    std::cerr << "Error trying to create Scanner: " << e.what() << '\n';
    return 65;
  }

  scanner->scanSource();
  for (auto t : scanner->tokens) {
    t.print();
    std::cout << '\n';
  }

  return 0;
}
