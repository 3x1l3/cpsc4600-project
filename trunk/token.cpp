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
     if (name == IDENTIFIER)
        return "IDENTIFIER";
     else if (name == NUMERAL)
        return "NUMERAL";
     else if (name == BADNUMERAL)
        return "BADNUMERAL";
     else if (name == BADNAME)
        return "BADNAME";
     else if (name == BADSYMBOL)
        return "BADSYMBOL";
     else if (name == BADCHAR)
        return "BADCHAR";
     else if (name == NEWLINE)
        return "NEWLINE";
     else if (name == NONAME)
        return "NONAME";
     else if (name == ENDOFFILE)
        return "ENDOFFILE";
     else if (name == DIV)
        return "DIV";
     else if (name == MOD)
        return "MOD";
     else if (name == SC)
        return "SC";
     else if (name == PLUS)
        return "PLUS";
     else if (name == MINUS)
        return "MINUS";
     else if (name == TIMES)
        return "TIMES";
     else if (name == DIVIDE)
        return "DIVIDE";
     else if (name == LB)
        return "LB";
     else if (name == RB)
        return "RB";
     else if (name == UNKNOWN)
        return "UNKNOWN";
     else if (name == PERIOD)
        return "PERIOD";
     else if (name == COMMA)
        return "COMMA";
     else if (name == LSB)
        return "LSB";
     else if (name == RSB)
        return "RSB";
     else if (name == AND)
        return "AND";
     else if (name == PIPE)
        return "PIPE";
     else if (name == TILDA)
        return "TILDA";
     else if (name == LT)
        return "LT";
     else if (name == GT)
        return "GT";
     else if (name == EQUALS)
        return "EQUALS";
     else if (name == FORSLASH)
        return "FORSLASH";
     else if (name == COLONEQUALS)
        return "COLONEQUALS";
     else if (name == BOTHSQUAREBRACKETS)
        return "BOTHSQUAREBRACKETS";
     else if (name == ARROW)
        return "ARROW";

     }

