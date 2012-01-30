/**
 * @brief The Definition(s) for the Token Object Class.
 *
 * @file token.cpp
 *
 * A token object provides three valuable pieces of information.
 * These are, a Type, a Value, and a Lexeme.
 * These are a Type enum, an integer and a string, respectively.
 * 
 * The token object represents a single PL Symbol - this can be
 * up to two characters in a row - or it represents a data type,
 * or an identifier, etc.
 *
 * The Token is used primarily by the Scanner, and by proxy,
 * by the Administrator in its functions revolving around the
 * scanner.
 *
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to 31st, 2011
 */

#include "token.h"

/** The default constructor, which assigns basic null-like values to the token. */
Token::Token() 
{
    value = 0;
    type = UNKNOWN;
    lexeme = "";
}

/**
 * @brief Creates a Token with a specified type, value and lexeme.
 *
 * A Token is constructed from the values given by the constructor
 * parameters.
 *
 * @param newType The given Type of token (if known).
 * @param newValue The value of our lexeme (if any).
 * @param newLexeme A string representing our Lexeme name.
 */
Token::Token(Type newType, int newValue, string newLexeme)
{
  type = newType;
  value = newValue;
  lexeme = newLexeme;
}

/** Return Functions give access to atomic Token data to outside actors. */
const int Token::getValue() const 
{
    return value;
}
const Type Token::getType() const 
{
    return type;
}
const string Token::getLexeme() const 
{
    return lexeme;
}


/** A set of mutators allow us to alter our Token data after construction. */
void Token::setType(Type newType) 
{
    type = newType;
}

void Token::setValue(int newValue) 
{
    value = newValue;
}

void Token::setLexeme(string newLexeme) 
{
    lexeme = newLexeme;
}

/**
 * @brief Returns a string representation of a token.
 * 
 * Token Data is transformed into a string of the format:
 * "< token-type, token-value, token-lexeme >"
 * and returned as a string.
 * @return string A string representing our token.
 */
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

/**
 * @brief Redefined Equivalency Operator for a Token.
 *
 * Defines the equivalency operator as a comparison between the lexeme, 
 * the value and the type of this token and the one it is compared to.
 *
 * @param first A token reference that we compare this one to.
 * @return a boolean indicating that it is equivalent (true) or not.
 */
bool Token::operator==(const Token& first) const
{
  if ((this->getLexeme() == first.getLexeme()) && (this->getValue() == first.getValue()) && (this->getType() == first.getType()))
	  return true;
  return false;
}

/**
 * @brief Redefined Non-Equivalency Operator for a Token.
 *
 * Returns the inverse of the equivalency operator.
 *
 * @param first A token reference that we compare this one to.
 * @return a boolean indicating that it is equivalent (false) or not.
 */
bool Token::operator!=(const Token& temp) const
{
  return !(*this == temp);
}

/**
 * @brief Returns a string representation of a token Data Type.
 * 
 * We return a string for an enumerated type, divined by subtracting
 * the base assignment amount (256) and using that number as an index.
 *
 * @param name A Type that is to be returned in string format.
 * @return A string version of the given type.
 */
string spellOutTypeName(Type name)
{
  int index = name-256;
  return typeSpelling[index];
}
