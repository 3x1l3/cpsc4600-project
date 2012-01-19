#ifndef TOKEN_H
#define TOKEN_H

class Token {

private:
    int value;
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