#include <cstdio>
#include <exception>
#include <fstream>
#include <iostream>
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>

class Scanner {
public:
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

  void scanString() {
    while (currentChar <= sourceString.length()) {
      printChar(getNextChar());
      std::cin.get();
    }

  }

private:
  std::string sourceString;

  int tokenStart = 0;
  int currentChar = 0;
  int line = 1;

  void readSourceFile(const std::string &fileName);
  void scanToken();
  const char getNextChar();
};

const char Scanner::getNextChar() {
  return sourceString[currentChar++];
}

void Scanner::scanToken() {

}

void Scanner::printChar(const char printable) {
  std::cout << printable << '\r';
  std::cout.flush();
}

void Scanner::printSourceString() {
  std::cout << sourceString << '\n';
}

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

  scanner->scanString();

  return 0;
}
