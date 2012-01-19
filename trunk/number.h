#ifndef NUMBER_H
#define NUMBER_H

class Number : public Token {
public:
    int value;
    Number(int v) : Token(NUM) {
        value = v;
    }
};

#endif