#include "SymbolTable.h"

void SymbolTable::addSymbol(const std::string &symbol, int address) {
    table[symbol] = address;
}
bool SymbolTable::contains(const std::string &symbol) const {
    return table.find(symbol) != table.end();
}
int SymbolTable::getAddress(const std::string &symbol) const {
    auto it = table.find(symbol);
    if (it != table.end()) {
        return it->second;
    }
    return -1; 
}