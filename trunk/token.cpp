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
const int Token::getValue() const {
    return value;
}
const std::string Token::getName() const {
    return name;
}

void Token::setName(std::string t) {
    name = t;
}

void Token::setValue(int t) {
    value = t;
}

std::string Token::toString()
{
    char val [10];
    sprintf(val,"%d", value); 
   
    std::string str;
    str = "<";
    str += name;
    str += ",";
    str += val;
    str += ">";
    return str;
}

 bool Token::operator==(const Token& first) const
{
  if (this->getName() == first.getName() && this->getValue() == first.getValue())
    return true;
  return false;
  
}

 bool Token::operator!=(const Token& temp) const
{
  return !(*this == temp);

}

