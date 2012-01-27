#ifndef SCANNER_H
#define SCANNER_H

#define NUMERAL_UPPER_BOUND 260000
#define NUMERAL_LOWER_BOUND 0
#define MAX_STRING_LENGTH 80

#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <vector>
#include "admin.h"
#include "token.h"
#include "symboltable.h"

using std::string;
using std::vector;

class Admin;

class Scanner {
private:
    int src_i;
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
    Admin* adminPtr;

public:
    Scanner(SymbolTable&, Admin&);
    ~Scanner() { };
    Token nextToken();
    void loadSource(string&);
    void scan();
    string getTokenizedString();
    bool inRange();
    bool isSpecial(char);
};

#endif
