/**
 * @brief The specification header for the overall Scanner component object.
 *
 * @file scanner.h
 * 
 * The Scanner is responsible for the direction of the peek and 
 * for smart construction of the tokens to be inserted into the symbol table.
 *
 * At that, the Scanner makes the appropriate entries into the symbol table 
 * for a given token, and also does the requisite checks to ensure that a 
 * token or symbol is valid.
 *
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to 31st, 2011
 **/
#ifndef SCANNER_H
#define SCANNER_H

/** Predefined constants that control the allowed range for our variables and types. */
// we have a upper bound of 2^16 so we can handle numbers for a 16 bit machine
#define NUMERAL_UPPER_BOUND 65536
#define NUMERAL_LOWER_BOUND 0
#define MAX_STRING_LENGTH 80

/** Basic Libraries needed by the Scanner object. */
#include <iostream>
#include <string>
#include <unistd.h>	//This is a basic Unix/Linux library, we don't seem to use it anywhere.
#include <stdlib.h>
#include <vector>

/** Interface headers needed by the Scanner */
#include "admin.h"
#include "token.h"
#include "symboltable.h"

/** Specific namespaces used by the Scanner. */
using std::string;
using std::vector;

/** Predeclare the Admin so we can use its functions safely. */
class Admin;


/**
 * @brief Scanner object reads chars, constructs tokens and contributes to parser/symboltable.
 *
 * It uses a collection of switches to handle certain symbols and mediations, but has a swath
 * of functions dedicated to particular handling of certain types of input symbols.
 *
 * The Scanner keeps track of its Peek and Index locations (relative to the PL source column)
 * and provides a suite of tokenization functions.
 *
 * Note in Advance: "src" and "source" often refer to the SOURCE File string passed all the way
 * (by reference or pointer) from Main. This is the PL program being loaded.
 */
class Scanner {

private:
    
    /** Used in the general parsing of the source file. */
    int src_i;
    char peek;

    /** Eventually refers to a PL source code string. */
    string* source;
    
    /** Used to move the peek and index and to increase the Admin column var. */
    bool readCharacter();
    /** Reads a character AND determines if it satisfies a desired value. */
    bool readCharacter(char);

    /** Functions for handling the given types of valid symbols encountered. */
    Token handleNumber();
    Token handleSymbol();
    Token handleCharString();
    
    /** A string of tokenized variables. Currently unused. */
    string tokenizedString;

    /** A vector of reserved words. Currently not used, but will be useful later. */
    vector<string> reservedWords;
    
    /** Pointers to necessary Main objects needed by the Scanner. */
    SymbolTable* symTable;
    Admin* adminPtr;
    


public:
    /** There should only be this one kind of constructor. */
    Scanner(SymbolTable&, Admin&);

    /** Basic, empty destructor. */
    ~Scanner() { };
    
    /** Goes to the next token and attempts to handle it; otherwise, it grabs the appropriate handler. */
    Token nextToken();
    

    void loadSource(string&);

    /** Unused internal Scanning function. Will be rewritten and used in the future. */
    void scan();
    
    /** Returns the tokenized string. */
    string getTokenizedString();
    
    /** Checks to see if we have reached the end of the range of the input string. Essentially, EOF(). */
    bool inRange();
    
    /** Checks if a character is a "special" (parsable) character. */
    bool isSpecial(char);
};

#endif