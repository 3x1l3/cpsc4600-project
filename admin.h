/**
 * @brief The specification for the Admin Class Object
 *
 * @file admin.h
 * 
 * The Administration Class is used to manage the overall 
 * compiler components and inter-connected objects.
 *
 * It also manages the error parent reporting, and tracks various
 * information such as column locations in order to accomodate said 
 * reporting.
 *
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to 31st, 2011
 */

#ifndef ADMIN_H
#define ADMIN_H

/** Standard headers used by the Administrator. */
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

/** Object definitions needed by the Admin */
#include "scanner.h"
#include "symboltable.h"
#include "parser.h"

/**
 * @brief This predefined is the maximum amount of errors the compiler will tolerate.
 *
 * Once this number is reached, the compiler "bails out".
 * We will make use of this later with our error handling.
 */
#define MAXERRORS 100

/** Shorthand Namespaces for keeping the code neat. */
using std::string;
using std::cout;
using std::endl;
using std::stringstream;

/** Used to avoid circular declaration problems.*/
class Scanner;
class Parser;


/**
 * @brief This is the main administrator class that manages the different compiler functions.
 *
 * This class mediates between the different analytical portions of the compiler.
 * The Administrator class ensures that the Scanner and Parser communicate using the same
 * set of data, such as the same Symbol Table(s), same line/column count, and the same
 * error data.
 *
 * As of now, the Administrator executes the scanning and parsing functions when needed
 * and supplies the correct data for each. It may also mediate for the Error Handling classes.
 */

class Admin {

private:
    /** 
     * Vectors used to store error details.
     * We store any strings of error messages that may occur, and where they occur.
     */
    std::vector<string> error_msgs;
    std::vector<int> error_line;

    /** We track the current line and column locations while reading through our File string. */
    int current_line;
    /** We store our errors in a StringStream for easy manipulation and conversion. */
    stringstream error_str;

    /** 
     * Symbol Table, Parser and Scanner object pointers.
     * This allows us to send symbol tables and scanners to and from other objects,
     * and makes it easier to manage memory and overflows.
     */
    SymbolTable* table;
    Scanner* scanner;
    Parser* parser;

    /**
     * The Index integer for our symbol table.
     * Increases per entry, removing the need to use the Symbol Table's iterator
     * functions.
     */
    int symTIndex;

    /** If a line cannot be scanned/parsed, this boolean is used to indicate such. */
    bool correctLine;

    /** 
     * Simple tracking of the number of errors encountered.
     * We abandon compilatiuon if this matches or exceeds the MAXERRORS definition.
     * @see MAXERRORS
     */
    int errorCount;
    
    /**
     *  Flag used to turn off/on token found printouts.
     */
    bool tokenFlag;
  
public:

    /**
     * @brief Base constructor for the Administrator.
     * 
     * The constructor takes in the finalized File string object pointer
     * from the Main file.
     * @param src variable
     */
    Admin(string&);

    /** Records the Error Message string into the Error vector. */
    void recordError(string);

    /** Executes the scanner function and returns an integer count of the errors found */
    int scan();
    
    /** The primary tokenizing function that can be called for scanning and parsing. */
    Token nextToken();

    /** 
     * A newline operation is performed as our current line is valid.
     * @see formal Definition of newLine()
     */
    void newLine();

    /** Our column count management functions allow us to report exact error locations */
    void increaseColumn();
    void resetColumn();
    bool checkMaxErrors();
    int column;
    
    /** Basic retrieval functions, used for debug. */
    int getColumnNumber();
    int getLineNumber();

};

#endif