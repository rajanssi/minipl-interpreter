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

const char Scanner::getNextChar() {
  // FIXME: PUT SOME SANE BOUNDS CHECKING HERE
  if (currentPosition_  + 1 > sourceString_.length()) {
      currentPosition_++;
      return '?';
  }
  return sourceString_[currentPosition_++]; 
}

const char Scanner::peek()
{
  // TODO: Add some error checking code
  return sourceString_[currentPosition_];
}

void Scanner::addToken(TokenType kind, const std::string &value)
{
  tokens.push_back({kind, value, line_});
}

void Scanner::scanToken()
{
  char character = getNextChar();
  switch (character)
  {
  case '(':
    addToken(TokenType::LEFT_PAR, "(");
    break;
  case ')':
    addToken(TokenType::RIGHT_PAR, ")");
    break;
  case '"':
    scanString();
    break;
  case '/': // Look one ahead to see if comment or operator
    if (peek() == '*')
    {
      scanComment();
    }
    else if (peek() == '/')
    {
      scanLineComment();
    }
    else
    {
      addToken(TokenType::OPERATOR, "/");
    }
    break;
  case '*':
      addToken(TokenType::OPERATOR, "*");
    break;
  case ':': // Scan one ahead to see if assignment (:=) or type (var x : int)
    if (peek() == '=')
    {
      getNextChar();
      // FIXME: SUBSTR IS INCORRECT HERE! AND POSSIBLY ELSEWHERE AS WELL!
      addToken(TokenType::ASSIGNMENT, sourceString_.substr(currentPosition_-2, 2));
    }
    else
    {
      addToken(TokenType::TYPE_DECLARATION, ":");
    }
    break;
  case '+':
    addToken(TokenType::OPERATOR, "+");
    break;
  case '-':
    addToken(TokenType::OPERATOR, "-");
    break;
  case '<':
    addToken(TokenType::OPERATOR, "<");
    break;
  case '=':
    addToken(TokenType::OPERATOR, "=");
    break;
  case '&':
    addToken(TokenType::OPERATOR, "&");
    break;
  case '!':
    // NOTE: Could probably make all operator cases fall through to this
    addToken(TokenType::OPERATOR, sourceString_.substr(currentPosition_-1, 1));
    break;
  case ';':
    addToken(TokenType::END_LINE, ";");
    line_++;
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
  case '\t':
  case ' ':
    break;
  case '\n':
    line_++;
    break;
  default:
    //addToken(TokenType::UNKNOWN, sourceString_.substr(currentPosition_-1, 1));
    break;
  }
}

void Scanner::scanLineComment()
{
}

void Scanner::scanComment()
{
}

void Scanner::scanNumber()
{
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
