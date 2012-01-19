#ifndef TOKEN_H
#define TOKEN_H

#include <string>

class Token {

protected:
    int value;

private:
    char symbol;

public:
    Token();
    Token(char);
    Token(int);
    ~Token() {}
    int getValue();
    char getSymbol();
    void set(char);
    void set(int);
    
};

#endif