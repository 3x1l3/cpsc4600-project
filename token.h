/**
 * @brief The specification/apparatus for the base Token object.
 *
 * @file token.h
 * 
 *	The Token class is the core object used by our scanner.
 * The Token is an object that contains the base information and
 * value and type and symbol data for a given character or rather,
 * for a given PL symbol/lexeme/type/etc.
 *
 * The Token is used primarily by the Scanner, and by proxy,
 * by the Administrator in its functions revolving around the
 * scanner.
 *
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to 31st, 2011
 */

#ifndef TOKEN_H
#define TOKEN_H

/** Standard Libraries used by the Token class object. */
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>

/** Temporary namespace shorthands - we use these specific ones everywhere token is used. */
using std::string; 
using std::cerr;
using std::cout;
using std::cin;
using std::endl;
using std::stringstream;

/**
 * @brief Enum for our token types.
 * Enumerated type "Type" that descrives the *type* of token being dealt with.
 * We go from enumerated values of 256 to 289 (after 255, the last of ASCII).
 */
enum Type 
{
    IDENTIFIER = 256, NUMERAL, BADNUMERAL, BADNAME,	// 256 - 259
    BADSYMBOL, BADCHAR, NEWLINE, NONAME, ENDOFFILE,	// 260 - 264
    DIV, MOD, PLUS, MINUS, TIMES, DIVIDE, UNKNOWN,	// 265 - 271
    PERIOD, COMMA, SC, LSB, RSB, AND, PIPE, TILDA,	// 272 - 279
    LT, GT, EQUALS, FORSLASH, LB, RB, COLONEQUALS,	// 280 - 286
    BOTHSQUAREBRACKETS, ARROW, COMMENT			// 287 - 289
};

/** @brief A string representation of enumerated types, for printing purposes. */
static string typeSpelling[] = 
{ 
	"IDENTIFIER", "NUMERAL", "BADNUMERAL", "BADNAME", 
	"BADSYMBOL", "BADCHAR", "NEWLINE", "NONAME", "ENDOFFILE",
	"DIV", "MOD", "PLUS", "MINUS", "TIMES", "DIVIDE", "UKNOWN",
	"PERIOD", "COMMA", "SC", "LSB", "RSB", "AND", "PIPE", "TILDA",
	"LT", "GT", "EQUALS", "FORSLASH", "LB", "RB", "COLONEQUALS",
	"BOTHSQUAREBRACKETS", "ARROW", "COMMENT"
};

/**
 * @brief Returns the corresponding string label for a given Type value.
 * User inputs one of our Type enums, and receives a String version of it.
 * @param name enum.
 * @return string String version of a Type.
 */
string spellOutTypeName(Type);


/**
 * @brief Token object that is made up of a lexeme (string), a value (integer) and a Type.
 *
 * The Token is the Strong/multivalued base type that is used by our Scanner, Parser
 * and their communicable Symbol Table.
 * 
 * It provides accessors and mutators for the associated value types.
 * The Token itself is represented by a "< token-type, token-value, token-lexeme >" format.
 * 
 */
class Token {

protected:
    /** We put the core Token values in a protected state in case we wish to subclass token. */
    int value;
    Type type;
    string lexeme;

private:


public:
	
    /** A base constructor for the Token class. Destructor unspecified. @see Token::Token() Definition. */
    Token();
    ~Token() {}

    /** 
     * @brief A special Token constructor, which we give it the Token data immediately.
     * Used when reconstructing the tokens.
     */
    Token(Type, int, string);
	


    /** A set of accessors for returning the bare lexical data of the Token. */
    const int getValue() const;
    const Type getType() const;
    const string getLexeme() const;

    /** A set of modifiers to change the Token's atomic data types. */
    void setType(Type);
    void setValue(int);
    void setLexeme(string);

    /** Converts the Token to a String in the form of <_,_,_> @see Token{};. */
    string toString();
    
    /** Overloaded operators to allow easy comparison between two tokens. */
    bool operator==(const Token&) const;
    bool operator!=(const Token&) const;
   
};

#endif