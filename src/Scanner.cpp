#include "Scanner.h"

#include <cstdlib>
#include <fstream>
#include <iterator>

void Scanner::scanSource() {
    while (currentPosition_ <= sourceString_.length()) {
        tokenStart_ = currentPosition_;
        scanToken();
    }
}

const char Scanner::getNextChar() {
    // TODO: PUT SOME SANE BOUNDS CHECKING HERE
    if (currentPosition_ + 1 > sourceString_.length()) {
        currentPosition_++;
        return '?';
    }
    return sourceString_[currentPosition_++];
}

const char Scanner::peek() {
    // TODO: Add some error checking code
    return sourceString_[currentPosition_];
}

void Scanner::addToken(TokenType kind, const std::string &value) {
    tokens.push_back({kind, value, line_});
}

void Scanner::scanToken() {
    char character = getNextChar();
    if (character == '_') {
        addToken(TokenType::UNKNOWN, "_");
        return;
    }
    if (idChars_.find(character) != idChars_.end()) {
        scanIdentifier(character);
        return;
    }
    switch (character) {
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
            if (peek() == '*') {
                // Scan until end of comment
                getNextChar();
                scanComment();
            } else if (peek() == '/') {
                // NOTE: line comment, can skip over the rest of the line
                while (peek() != '\n')
                    getNextChar();
                break;
            } else {
                addToken(TokenType::OPERATOR, "/");
            }
            break;
        case '*':
            addToken(TokenType::OPERATOR, "*");
            break;
        case ':': // Scan one ahead to see if assignment (:=) or type (var x : int)
            if (peek() == '=') {
                getNextChar();
                addToken(TokenType::ASSIGNMENT, ":=");
            } else {
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
            addToken(TokenType::OPERATOR, "!");
            break;
        case ';':
            addToken(TokenType::END_LINE, ";");
            // line_++;
            break;
        case '.':
            if (peek() == '.') {
                getNextChar();
                addToken(TokenType::DOTDOT, "..");
                break;
            } else {
                addToken(TokenType::UNKNOWN, ".");
            }
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
            scanNumber(character);
            break;
        case '\t':
        case ' ':
            break;
        case '\n':
            line_++;
            break;
        default:
            //addToken(TokenType::UNKNOWN, sourceString_.substr(currentPosition_ - 1, 1));
            break;
    }
}

void Scanner::scanIdentifier(char starter) {
    char nextChar = peek();
    std::string identifier = "";
    identifier += starter;
    // NOTE: nextChar must be AZaz, _, or 0-9
    while (idChars_.find(nextChar) != idChars_.end() || nextChar == '_' ||
           (nextChar > 47 && nextChar < 58)) {
        identifier += getNextChar();
        nextChar = peek();
    }
    if (keywords_.find(identifier) != keywords_.end()) {
        addToken(TokenType::KEYWORD, identifier);
        return;
    }
    if (identifier == "true" || identifier == "false") {
        addToken(TokenType::BOOLEAN, identifier);
        return;
    }
    addToken(TokenType::IDENTIFIER, identifier);
}

void Scanner::scanComment() {
    while (currentPosition_ < sourceString_.length()) {
        if (getNextChar() == '*' && peek() == '/') {
            getNextChar();
            return;
        }
    }
}

void Scanner::scanNumber(char starter) {
    std::string number = "";
    number += starter;
    char nextNum = peek();
    while (nextNum > 47 && nextNum < 58) {
        number += getNextChar();
        nextNum = peek();
    }

    addToken(TokenType::NUMBER, number);
}

void Scanner::scanString() {
    char nextChar = ' ';
    while (nextChar != '"' && currentPosition_ <= sourceString_.length()) {
        nextChar = getNextChar();
        if (nextChar == '\\') {
            currentPosition_++;
        }
    }

    addToken(TokenType::STRING,
             sourceString_.substr(tokenStart_+1, currentPosition_ - 2 - tokenStart_));
}

const void Scanner::fillIdCharacterTable() {
    for (char c = 'A'; c <= 'Z'; c++) {
        idChars_.insert(c);
    }
    for (char c = 'a'; c <= 'z'; c++) {
        idChars_.insert(c);
    }
}

const void Scanner::fillKeywordTable() {
    keywords_.insert("var");
    keywords_.insert("for");
    keywords_.insert("end");
    keywords_.insert("in");
    keywords_.insert("do");
    keywords_.insert("read");

    keywords_.insert("print");
    keywords_.insert("int");
    keywords_.insert("string");
    keywords_.insert("bool");
    keywords_.insert("assert");

    keywords_.insert("if");
    keywords_.insert("else");
}

void Scanner::printSourceString() { std::cout << sourceString_ << '\n'; }

void Scanner::readSourceFile(const std::string &fileName) {
    std::ifstream file(fileName);

    if (!file) {
        throw std::runtime_error("Failed to read file " + fileName);
    }

    while (file) {
        std::string inputStream;
        std::getline(file, inputStream);
        sourceString_ += inputStream;
        sourceString_ += '\n';
    }
}
