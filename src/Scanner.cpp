#include "Scanner.h"

#include <fstream>

void Scanner::scanSource()
{
  while (currentPosition_ <= sourceString_.length())
  {
    tokenStart_ = currentPosition_;
    scanToken();
  }
}

const char Scanner::getNextChar() { return sourceString_[currentPosition_++]; }

void Scanner::addToken(TokenType kind, const std::string &value)
{
  tokens.push_back({kind, value, line_});
}

void Scanner::scanToken()
{
  char character = getNextChar();
  switch (character)
  {
  case '"':
    scanString();
    break;
  case '+':
  case '-':
  case '*':
  case '/':
  case '<':
  case '=':
  case '&':
  case '!':
    addToken(TokenType::OPERATOR, "operator FIXME");
    break;
  case ';':
    line_++;
    addToken(TokenType::END_LINE, ";");
    break;
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '8':
  case '9':
    // NOTE: Need  to do some peeking here, for long integers and floats
    scanNumber();
    break;
  case '\n':
    line_++;
    break;
  default:
    break;
  }
}

void Scanner::scanNumber() {
  std::cout << "num literal!" << '\n';
}

void Scanner::scanString()
{
  char nextChar = ' ';
  while (nextChar != '"' && currentPosition_ <= sourceString_.length())
  {
    nextChar = getNextChar();
    if (nextChar == '\\')
    {
      currentPosition_++;
    }
  }

  addToken(TokenType::STRING,
           sourceString_.substr(tokenStart_, currentPosition_ - tokenStart_));
}

void Scanner::printChar(const char character)
{
  std::cout << character << '\r';
  std::cout.flush();
}

void Scanner::printSourceString() { std::cout << sourceString_ << '\n'; }

void Scanner::readSourceFile(const std::string &fileName)
{
  std::ifstream file(fileName);

  if (!file)
  {
    throw std::runtime_error("Failed to read file " + fileName);
  }

  while (file)
  {
    std::string inputStream;
    std::getline(file, inputStream);
    sourceString_ += inputStream;
  }
}
