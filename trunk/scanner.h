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
#include <vector>
#include "token.h"
#include "symboltable.h"

using std::string;
using std::vector;


class Scanner {
private:
    int src_i, currentCharacter;
    string* source;
    char peek;
    bool readCharacter();
    bool readCharacter(char);
    Token handleNumber();
    Token handleSymbol();
    Token handleCharString();
    string tokenizedString;
    vector<string> reservedWords;
    SymbolTable* symTable;

public:
    Scanner(SymbolTable&);
    ~Scanner() { };
    Token nextToken();
    void loadSource(string&);
    void scan();
    string getTokenizedString();
    bool inRange();
    bool isSpecial(char);
};

#endif
