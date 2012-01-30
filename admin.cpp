/**
 * @brief The formal definition for the Admin Class Object.
 *
 * @file admin.cpp
 * 
 * The Administration Class is used to manage the overall 
 * compiler components and inter-connected objects.
 * 
 * It records and outputs error information, and manages the 
 * communication and operation of the subsequent Scanner and Parser
 * components.
 * 
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to 31st, 2011
 **/

#include "admin.h"

/**
 * @brief Constructor for the Admin class.
 * 
 * Sets the default values of our class variables,
 * then creates the symbol table and scanner.
 * 
 * Uses the source string "src", which is the File string
 * reference containing the entire PL program.
 * 
 * @param src The PL program in the File string.
 */
Admin::Admin(string& src)
{
    current_line = 1;
    column = 0;
    symTIndex = 0;
	errorCount = 0;
	/** correctLine could technically be used a constructor sentinel in the future. */
    correctLine = true;

	/** We send the symbol table to the Scanner, along with a reference to the Administrator. */
    table = new SymbolTable();
    scanner = new Scanner(*table, *this);

	/** The reference to the "File" string is sent to the scanner. */
	scanner->loadSource(src);

}

/**
 * @brief Function outputs the error and adjusts vars proceedingly.
 * 
 * recordError is passed a string describing a recorded error that
 * has occured.
 *
 * The function adjusts internal values accordingly, and outputs the given
 * message to the screen.
 * As of now, only one error per line is recorded, then we dump the line.
 *
 * @param msg The error message to be recorded.
 */
void Admin::recordError(string msg)
{
    if (correctLine) {
        correctLine = false;
        cerr << msg << endl;
        errorCount++;
    }

}

/**
 * @brief The Admin function that operates the Scanner and details errors as necessary.
 * 
 * We check for the various heuristics of the scanner, and then call its functions
 * (such as nextToken()) as needed to read through a file and construct the symbol
 * 
 * Essentially, we keep running the scanner (advancing by char/column)
 * and recording any errors we enounter and we abort the entire process 
 * if our error threshold is reached.
 * 
 * We output the token, and ensure that we stay within "file" range (string, technically).
 *
 * Finally, we return an integer that counts the number of errors.
 * Thus, if a 0 means our scanner has scanned successfully.
 * 
 * @pre We have a file string of a PL program and an empty symbol table.
 * @post We will have a complete symbol table and have scanned our PL file.
 * @return An integer count of errors encountered.
 */

int Admin::scan()
{
	/** An empty file has no errors, so we return a 0 (successful scan of nothing). */
    if(scanner->inRange() == false)
      return 0;
  
	/** We store our errors in a StringStream for easy manipulation and conversion. */
    stringstream error_str;
    
	/** 
	 * We create an internal Token "tok" and could store this in an array or the symbol table
	 * if desired. In this case, it is a temporary one for Administrative purposes.
	 * We use this specific token instance to check its type and if it represents an error.
	 */
	Token tok;

	/** Type enum used to save on function calls, @see token.h "enum type{}" */
	Type currentTokenType;


	/**
	 * @brief Keep scanning and check for errors until we reach the end of the file.
	 *
	 * This do-while loop sends the scanner to the next char in the file until the scanner
	 * reports that it is out of range (at the end of the file).
	 * This is done by telling the scanner to advance to the next token per instance,
	 * and since we verified that the file is not empty (see above), we do this at least once.
	 */
    do {
        tok = scanner->nextToken();
		currentTokenType = tok.getType();

		/** If we have encountered a "new line" token, we account for it and reset the line-error boolean. */
        if (tok.getType() == NEWLINE) {
            newLine();
        }

		/**
		 * We check if the current token is an error token, and report it to the user.
		 * At the moment, the program only reports one error per line. 
		 * The code is boilerplate for the rest of these, so they could be a switch statement.
		 */
        else if (/*currentTokenType == BADCHAR ||*/ currentTokenType == BADSYMBOL) 
		{
		  errorCount++;
		  error_str << "Illegal character " << current_line << ":" << column << endl;
		  recordError(error_str.str());
		  
		  if(checkMaxErrors())
			  return errorCount;
        } 

        else if (currentTokenType == BADNUMERAL) 
		{
		  errorCount++;
		  error_str << "Numeral overflow on " << current_line << ":" << column << endl;
		  recordError(error_str.str());
		  
		  if(checkMaxErrors())
			  return errorCount;
        }

        else if (currentTokenType == BADNAME) 
		{
		  errorCount++;
		  error_str << "Identifier cannot exceed 80 characters " << current_line << ":" << column << endl;
		  recordError(error_str.str());
		  
		  if(checkMaxErrors())
			  return errorCount;
		}

		/** This outputs the Token to the Screen in a < ___,___,___> format. */
        cout << tok.toString() << endl;
    }
    while (scanner->inRange()) ;
    
	/** We return an "all clear" digit of 0 if there were no errors. The rest is handled in the main. */
	if (errorCount == 0)
		return 0;
	else
		cerr << "\nEncountered: " << errorCount << " errors." << endl;
    return -1;
}


/** Used to adjust the current line, and also for skipping lines (errors, comments, etc.) */
void Admin::newLine()
{
    current_line++;
    correctLine = true;
}

/** Our column count is increased. We use it to track error and token positions. */
void Admin::increaseColumn()
{
    column++;
}

/** The default "left-aligned" column is 1, we reset it when we reach the end of a line. */
void Admin::resetColumn()
{
    column = 1;
}


/**
 * @brief Checks if we have reached the maximum number of errors.
 * 
 * If we have reached the maximum number of errors, we return a true statement as
 * well as letting the operator/user know this has happened.
 * Used to shorten code above.
 *
 * @return A boolean indicating if we have too many errors.
 */
bool Admin::checkMaxErrors()
{
	if(errorCount >= MAXERRORS)
	{
	  cerr<<"Error threashold limit reached"<<endl;
	  return true;
	}
	else
	  return false;
}