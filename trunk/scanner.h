#ifndef SCANNER_H
#define SCANNER_H

#define NUM 256
#define ID 257
#define TRUE 258
#define FALSE 259
#define ASSIGN 260

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include "token.h"
#include "symboltable.h"


class Scanner {
private:
    int line, src_i;
    std::string* source;
    char peek;
    void readCharacter();
    bool readCharacter(char);
    Token handleNumber();
    Token handleSymbol();
    Token handleCharString();
    SymbolTable* table;

public:
    Scanner(SymbolTable*);
    ~Scanner() { };
    Token nextToken();
    void loadSource(std::string&);
    void scan();
};

#endif
