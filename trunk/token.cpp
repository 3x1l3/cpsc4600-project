#include "token.h"

Token::Token() {
    value = 0;
    name = ' ';
}

Token::Token(std::string t) {
    setName(t);
}

Token::Token(int t) {
    setValue(t);
}

Token::Token(std::string newName, int newValue)
{
  name = newName;
  value = newValue;
}
int Token::getValue() {
    return value;
}
std::string Token::getName() {
    return name;
}

void Token::setName(std::string t) {
    name = t;
}

void Token::setValue(int t) {
    value = t;
}

