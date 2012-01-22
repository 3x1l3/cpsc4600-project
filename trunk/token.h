#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

class Token {

protected:
    int value;

private:
    std::string name;

public:
    Token();
    Token(std::string);
    Token(int);
    Token(std::string, int);
    ~Token() {}
    const int getValue() const;
    const std::string getName() const;
    void setName(std::string);
    void setValue(int);
    std::string toString();
    
 bool operator==(const Token&) const;
 bool operator!=(const Token&) const;
   
};

#endif