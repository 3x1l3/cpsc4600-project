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
    //start at negative one casue there is always one more column than actualy charcter
    column = -1;
    symTIndex = 0;
    errorCount = 0;
    /** correctLine could technically be used a constructor sentinel in the future. */
    correctLine = true;

    /** We send the symbol table to the Scanner, along with a reference to the Administrator. */
    table = new SymbolTable();
    scanner = new Scanner(*table, *this);
    
    /** The reference to the "File" string is sent to the scanner. */
    scanner->loadSource(src);
    
    /** Create the base Parser object with a pointer back to here to allow it to access the scanner-> */
    parser = new Parser(*this, *table);

    /** A flag for managing the output of TOKEN debug information. Set to TRUE to see said information. */
    tokenFlag = false;

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
    if (correctLine) 
    {
      correctLine = false;
      error_str.str("");
      error_msgs.push_back(msg);
    }

}

/**
 * @brief The Admin function that operates the Scanner and details errors as necessary.
 * 
 * We check for the various heuristics of the scanner, and then start the Parser.
 * The parser itself then has direct control fo the scanner, though this
 * function ensures that the scanner or parser's error thresholds are accounted for.
 * 
 * 
 * We output the token, and ensure that we stay within "file" range (string, technically).
 *
 * Finally, we return an integer that counts the number of errors.
 * Thus, if a 0 means our scanner has scanned successfully and the parser has not failed.
 * 
 * @pre We have a file string of a PL program and an empty symbol table.
 * @post We will have a complete symbol table and have scanned our PL file. Our file will be parsed.
 * @return An integer count of errors encountered.
 */

int Admin::scan()
{
  if(scanner->inRange() == false)
    return 0;
  
  /** Run the Parser! The parser itself will also manage the error count. */
  cout << "\nParsing Started." << endl;
  // We're finally at this stage!
  cout << "Scope and Type Check Started" << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << endl;
  parser->run();
  
  /** We return an "all clear" digit of 0 if there were no errors. The rest is handled in the main. */
  if (errorCount == 0)
  {
    /* Test output of our inter code. 
      cout << "\nAt the end of Parsing. Assembling Code Reprint: \n";
      cout << assemblerStream.str() << endl;
    */
    return 0;
  }
  else
  {
    cerr << "\nEncountered: " << errorCount << " errors." << endl;
    for(unsigned int i = 0; i < error_msgs.size(); i++)
    {
      cerr << "Error " << i+1 << ": " << error_msgs.at(i) << endl;
    }
  }
  return -1;

}

/**
 * @brief Primary tokenizing management function as used by the Admin to construct and retrieve tokens.
 * 
 * This function receives a Token from the scanner and handles errors/exceptions at the same time.
 * It is called for Scanning and Parsing, respectively.
 * 
 * @pre The scanner is at a specific index i
 * @post The index and lookahead are at i+1 and i+2 and a token has been constructed.
 * @return A specific token as pertains to the given type from the scanner
 */
Token Admin::nextToken()
{
    if(!scanner->inRange())
    {
      return Token(ENDOFFILE, -1, "$");
    }
  
    Token tok;
    Type currentTokenType;

    tok = scanner->nextToken();
    currentTokenType = tok.getType();

    /**
     * We check if the current token is an error token, and report it to the user.
     * At the moment, the program only reports one error per line. 
     * The code is boilerplate for the rest of these, so they could be a switch statement.
     */
    if (currentTokenType == BADSYMBOL) 
    {
      errorCount++;
      error_str << "Illegal ' "<<tok.getLexeme()<<" ' character detected. Check line " << current_line << ", column " << column <<"."<< endl;
      recordError(error_str.str());
		
      if(checkMaxErrors())
	return Token(ENDOFFILE, -1, "$");
    } 
    else if (currentTokenType == BADNUMERAL) 
    {
      errorCount++;
      error_str << "Numeral overflow found. Numerals cannot exceed 65536. Check line " << current_line << ", column " << column <<"."<< endl;
      recordError(error_str.str());
		
      if(checkMaxErrors())
	return Token(ENDOFFILE, -1, "$");
    }
    else if (currentTokenType == BADNAME) 
    {
      errorCount++;
      error_str << "Identifier cannot exceed 80 characters. Check line " << current_line << ", column " << column <<"."<< endl;
      recordError(error_str.str());
		
      if(checkMaxErrors())
	return Token(ENDOFFILE, -1, "$");
      
    }
    /** This outputs the Token to the Screen in a < ___,___,___> format. */
    if (tokenFlag)
      cout << tok.toString() <<" \t\t Found at line " << current_line << ", column " << column << "."<< endl;

    return tok;

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
    column = -1;
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
    cerr << "Error threashold limit reached. Bailing out of scanning." << endl;
    return true;
  }
  else
    return false;
}

/** Retrieve the Column number in raw/unchanged/unmodified form. */
int Admin::getColumnNumber()
{
  return column;
}

/** Send back the line number without edits. */
int Admin::getLineNumber()
{
  return current_line;
}

/** 
 * @brief Singular Emit Function
 * Outputs the single string param of backend code.
 * Stores in primary backend StringStream.
 */
void Admin::emit(string str) 
{
  assemblerStream << str << endl;
  cout << str << endl;
}

/** 
 * @brief Double Emit Function
 * Outputs two-param interp code.
 * Stores in primary backend StringStream.
 */
void Admin::emit2(string str, int i) 
{
  assemblerStream << str << endl << i << endl;
  cout << str << endl;
  cout << i << endl;
}

/** 
 * @brief Trinary Emit Function
 * Outputs three-piece code.
 * Stores in primary backend StringStream.
 */
void Admin::emit3(string str, int i, int j) 
{
  assemblerStream << str << endl << i << endl << j << endl;
  cout << str << endl;
  cout << i << endl;
  cout << j << endl;
}
