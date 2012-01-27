#include "token.h"

Token::Token() {
    value = 0;
    type = UNKNOWN;
    lexeme = "";
}


Token::Token(Type newType, int newValue, string newLexeme)
{
  type = newType;
  value = newValue;
  lexeme = newLexeme;
}
const int Token::getValue() const {
    return value;
}
const Type Token::getType() const {
    return type;
}
const string Token::getLexeme() const {
    return lexeme;
}

void Token::setType(Type t) {
    type = t;
}

void Token::setValue(int t) {
    value = t;
}

void Token::setLexeme(string newLexeme) {
    lexeme = newLexeme;
}

string Token::toString()
{
  string newString  = "";
  char val [10];
  sprintf(val,"%d", value); 
  newString += "<";
  newString += spellOutTypeName(type);
  newString += ",";
  newString += val;
  newString += ",";
  newString += lexeme;
  newString += ">";
  return newString;
}

 bool Token::operator==(const Token& first) const
{
  if ((this->getLexeme() == first.getLexeme()) && (this->getValue() == first.getValue()) && (this->getType() == first.getType()))
    return true;
  return false;
  
}

 bool Token::operator!=(const Token& temp) const
{
  return !(*this == temp);

}

string spellOutTypeName(Type name){
  
    int index = name-256;
    
    return typeSpelling[index];

     }

