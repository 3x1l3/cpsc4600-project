#ifndef TOKEN_H
#define TOKEN_H

#include <string>

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
    int getValue();
    std::string getName();
    void setName(std::string);
    void setValue(int);
    
};

#endif