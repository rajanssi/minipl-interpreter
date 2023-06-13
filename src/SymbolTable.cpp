#include <iostream>
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

const std::string &Symbol::getId() {
    return id;
}

const SymbolType Symbol::getType() {
    return type;
}

void Symbol::print() {
    switch (type) {
        case SymbolType::INT:
            std::cout << "[ " << id << ", int, " << i_ << " ]\n";
            break;
        case SymbolType::STRING:
            std::cout << "[ " << id << ", string, " << s_ << " ]\n";
            break;
        case SymbolType::BOOL:
            std::cout << "[ " << id << ", bool, " << b_ << " ]\n";
            break;
    }
}

void SymbolTable::printSymbols() {
    std::cout << "Printing symbols...\n";
    for (auto &symbol: symbolTable_) {
        symbol.second->print();
    }
}

void SymbolTable::addSymbol(std::string &id, Symbol *symbol) {
    if (symbolTable_.find(id) != symbolTable_.end()) {
        std::cerr << "Variable " << id << " already declared";
        std::abort();
    }
    symbolTable_[id] = symbol;
}

void SymbolTable::setSymbolValue(std::string &id, std::string value) {
    symbolTable_[id]->setValue(value);
}

void SymbolTable::setSymbolValue(std::string &id, int value) {
    symbolTable_[id]->setValue(value);
}

void SymbolTable::setSymbolValue(std::string &id, bool value) {
    symbolTable_[id]->setValue(value);
}

Symbol &SymbolTable::getSymbol(std::string id) {
    if (symbolTable_.find(id) == symbolTable_.end()) {
        std::cerr << "Symbol " << id << " has not been declared";
        std::abort();
    }

    return *symbolTable_[id];
}

