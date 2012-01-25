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



class Scanner {
private:
    int line, src_i, currentCharacter, col;
    std::string* source;
    char peek;
    bool readCharacter();
    bool readCharacter(char);
    Token handleNumber();
    Token handleSymbol();
    Token handleCharString();
    std::string tokenizedString;
    std::vector<char> symbols;
    std::vector<std::string> symbolStrings;
    std::vector<std::string> reservedWords;
    
    int checkSymbol(char symbol);

public:
    Scanner();
    ~Scanner() { };
    Token nextToken();
    void loadSource(std::string&);
    void scan();
    std::string getTokenizedString();
    bool inRange();
};

#endif