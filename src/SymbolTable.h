#pragma once

#include <unordered_map>
#include <string>

enum class SymbolType {
    INT,
    STRING,
    BOOL
};

class Symbol {
public:
    Symbol(SymbolType type, std::string id, int value);
    Symbol(SymbolType type, std::string id, std::string value);
    Symbol(SymbolType type, std::string id, bool value);

    void setValue(std::string value);
    void setValue(int value);
    void setValue(bool value);

    std::string getStringValue();
    int getIntValue();
    bool getBoolValue();

    const std::string& getId();

    const SymbolType getType();

private:
    SymbolType type;
    std::string id;

    int i_;
    std::string s_;
    bool b_;
};


class SymbolTable {

public:
    // TODO: get, set, etc
    void addSymbol(std::string id, Symbol symbol);

    void setSymbolValue(std::string id, std::string value);
    void setSymbolValue(std::string id, int value);
    void setSymbolValue(std::string id, bool value);

    Symbol getSymbol(std::string id);

private:
    std::unordered_map<std::string, Symbol> symbolTable_;

    void addSymbol(std::string id, Symbol &symbol);
};

