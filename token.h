#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
using namespace std; 
enum Type {IDENTIFIER=256, NUMERAL, BADNUMERAL, BADNAME,           // 255 - 259
             BADSYMBOL, BADCHAR, NEWLINE,NONAME,            // 260 - 263
             ENDOFFILE, DIV, MOD,                // 264 - 267
             PLUS, MINUS, TIMES, DIVIDE, UNKNOWN,                    // 268 - 271
             PERIOD, COMMA, SC, LSB, RSB, AND, PIPE, TILDA, //272 - 281
	     LT, GT, EQUALS, FORSLASH, LB, RB, COLONEQUALS, BOTHSQUAREBRACKETS, ARROW, COMMENT}; //282 - 290     
	   static string typeSpelling[] = { "IDENTIFIER", "NUMERAL", "BADNUMERAL", "BADNAME", "BADSYMBOL","BADCHAR","NEWLINE","NONAME","ENDOFFILE","DIV", "MOD","PLUS","MINUS","TIMES","DIVIDE","UKNOWN","PERIOD","COMMA","SC","LSB","RSB",
	   "AND","PIPE","TILDA","LT","GT","EQUALS","FORSLASH", "LB", "RB", "COLONEQUALS", "BOTHSQUAREBRACKETS", "ARROW", "COMMENT"};
string spellOutTypeName(Type);
	     
 
class Token {

protected:
    int value;
    Type type;
    string lexeme;

private:


public:
    Token();
    Token(Type, int, std::string);
    ~Token() {}
    const int getValue() const;
    const Type getType() const;
    const std::string getLexeme() const;
    void setType(Type);
    void setValue(int);
    void setLexeme(std::string);
    std::string toString();
    
 bool operator==(const Token&) const;
 bool operator!=(const Token&) const;
   
};

#endif