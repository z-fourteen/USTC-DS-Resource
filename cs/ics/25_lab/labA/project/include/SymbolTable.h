#pragma once
#include <string>
#include <unordered_map>

class SymbolTable {
public:
    void addSymbol(const std::string &symbol, int address);
    bool contains(const std::string &symbol) const;
    int getAddress(const std::string &symbol) const;
    const std::unordered_map<std::string, int> &getTable() const{
        return table;
    }
private:
    std::unordered_map<std::string, int> table;
};