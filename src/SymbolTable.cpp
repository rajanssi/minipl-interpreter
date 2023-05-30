#include "SymbolTable.h"

Symbol::Symbol(SymbolType type, std::string id, std::string value) : type(type), id(id), s_(value) {};
Symbol::Symbol(SymbolType type, std::string id, int value) : type(type), id(id), i_(value) {};
Symbol::Symbol(SymbolType type, std::string id, bool value) : type(type), id(id), b_(value) {};

void Symbol::setValue(std::string value) {
    s_ = value;
}

void Symbol::setValue(int value) {
    i_ = value;
}

void Symbol::setValue(bool value) {
    b_ = value;
}

std::string Symbol::getStringValue() {
   return s_;
}

int Symbol::getIntValue() {
    return i_;
}

bool Symbol::getBoolValue() {
    return b_;
}

const std::string& Symbol::getId() {
    return id;
}

const SymbolType Symbol::getType() {
    return type;
}

void SymbolTable::addSymbol(std::string id, Symbol symbol) {
}

void SymbolTable::setSymbolValue(std::string id, std::string value) {
    symbolTable_.at(id).setValue(value);
}

void SymbolTable::setSymbolValue(std::string id, int value) {
    symbolTable_.at(id).setValue(value);
}

void SymbolTable::setSymbolValue(std::string id, bool value) {
    symbolTable_.at(id).setValue(value);
}

Symbol SymbolTable::getSymbol(std::string id) {
    return symbolTable_.at(id);
}

