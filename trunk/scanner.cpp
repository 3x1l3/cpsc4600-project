/**
 * @brief The definition(s) for the Scanner object.
 *
 * @file scanner.cpp
 * 
 * The scanner coordinates the reading of characters from the file
 * and the construction of tokens from those characters.
 * It mediates "type" disputes and potential flawed or irregular inputs,
 * and constructs the appropriate tokens to address these situations.
 *
 * Finally, it facilitates communication between the Parser and the 
 * original source string.
 * Future extensibility is afforded by accessors/mutators and extensible
 * switches for types.
 *
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to 31st, 2011
 **/

#include "scanner.h"

/**
 * @brief Constructs the Scanner object using a symboltable and admin reference.
 * 
 * The scanner is given necessary Symbol Table and Admin class object
 * references, and assigns a class-local pointer to each of them.
 *
 * Default/beginning values are assigned to the index, the peek variable
 * and the source string ptr.
 * 
 * We insert a predefined list of reserved words into a vector for future use and
 * easy expansion.
 *
 * @param table A reference to the Symbol table from the Administrator object.
 * @param admin A reference to the Administrator class object in main.
 */
Scanner::Scanner(SymbolTable& table, Admin& admin) 
{    
    //Source index, per character
    src_i = 0;
    source = NULL;
    peek = ' ';
    symTable = &table; 
    adminPtr = &admin;

    //is not used currently, may be usefule later
    string reservedWordsArray[] = { "begin", "end", "const", "array", "integer", "Boolean", "proc", "skip", "read",
                                     "write", "call", "if", "do", "fi", "od", "false", "true" };
    //not used now, may be useful later
    reservedWords.insert(reservedWords.begin(), reservedWordsArray, reservedWordsArray+(sizeof(reservedWordsArray) / sizeof(string)));
}

/**
 * @brief Links the Scanner's "source" string to the original PL source 
 * code File from main.
 *
 * Load the source into a local string pointer/reference, saving memory 
 * and time on larger source files.
 *
 * If the source file isn't empty, we then kick off our indexes by reading 
 * a character for our scanner.
 * @param src
 */
void Scanner::loadSource(string& src) 
{
    source = &src;
   
    //set ch to the first item in the source
    if (source->size() > 0 )
    {
      readCharacter();
    }
}

/**
 * @brief Sets the peek value to the next character if not out of bounds.
 * 
 * Sets our peek to the value at our index (so, the offset is +1)
 *	This is one in front of src_i itself.
 * We increase src_i to accomodate the next read and peek.
 * @return bool A boolean indicating if we're out of range after reading.
 */
bool Scanner::readCharacter()
{
    if (inRange())
    {
      peek = source->at(src_i);
      src_i++;
      adminPtr->increaseColumn();
      return true;
    }
    else
    {
        /* std::cout << "out of bounds read" << std::endl;
         std::cout << "peek = " << peek <<std::endl;
         std::cout << "src_i = "<< src_i <<std::endl;*/
      return false;
    }
}


/**
 * @brief Verifies that the next character matches a desired character.
 *
 * Overload of readch, checks to see if the next peeked character matches the parameter
 * Its prime difference is that the return var is based off of a parameter, rather than
 * off of whether the range end has been reached.
 *
 * @param c A character that we expect to find (ie a '>' after a '-') 
 * @return bool A boolean for if the peeked character matches the parameter character.
 */
bool Scanner::readCharacter(char c) 
{
    readCharacter();
    if (peek != c) 
      return false;
    peek = ' ';
    return true;
}

/**
 * @brief Handles the Token reading, mediation and assignment(s).
 *
 * This function is currently represents the core functionality of the scanner.
 * It mediates input string by choosing the appropriate token handler
 * such as "handleNumber".
 *
 * Additionally, it makes sure that whitespace, tabs, comments, and newlines
 * are appropriately parsed.
 * 
 * This latter mediation is handled by reading a new character until an 
 * appropriate "non-skippable" character/possible token is encountered.
 * 
 * Ultimately, it creates a Token to return to its caller.
 *
 * @pre A Token is expected, we are at the start of a statement block.
 * @post A Token has been created and the peek/index are at the start of a new statement.
 * @return Token The next token read from the PL Source File.
 */
Token Scanner::nextToken() 
{
    bool comment = false;

    /** 
     * This block is used to skip the whitespaces and associated formatting
     * characters such as tab.
     */
    do
    {
      if ( peek == ' ' || peek == '\t')
      {
	 
           //do nothing if it's a tab or whitespace.
		   //the While check will increment the peek/index.
      }
      else if (peek == '\n')
      {
	/**
	 * We've reached a newline, so the column will now go back to left/'1'.
	 * We read this character to check if it's a comment.
	 * If it's a comment, we can skip this line and make a Comment token.
	 * Otherwise, we construct a Newline token.
	 */
	adminPtr->resetColumn();
	adminPtr->newLine();
	readCharacter();
			
	if(comment == true)
	{
	  comment = false;
	  return Token (COMMENT, -1, spellOutTypeName(COMMENT));
	  
	}
	else
	{
	  comment = false;
	  return Token(NEWLINE, -1, spellOutTypeName(NEWLINE));
	}
      }
      else if (peek == '$')
      {
	comment = true;   
      }
      /** Nothing above was found, but it could be a special character within a comment. */
      else
      {
	/** But if it's not in a comment, then that means we must handle it. */
	if (comment == false)
	{
	  break;
        }
      }
    } while (readCharacter() && ( peek == ' ' || peek == '\t' || peek == '\n' ||peek == '$' || comment == true));

    //Handle Digits
    if (isdigit(peek))
    {
      return handleNumber();
    }
    //Handle Letters
    if (isalpha(peek))
    {
      return handleCharString();
    }
    //handles special symbols
    if (isSpecial(peek))
    {
      return handleSymbol();
    }
    /** 
     * We've encountered a broken symbol.
     * Increment our index' and return an error token.
     */
    else
    {
      //originally, we return a &temp == peek character.
      //this gets us garbage.
      string peekChar = "";
      peekChar += peek;
      readCharacter();
      return Token(BADSYMBOL, -1, peekChar);
    }

    /** An unknown symbol has been encountered. This should almost never happen. */
    cout<< peek <<endl;
    return Token(UNKNOWN, -1, "unknown character");
}

/**
 * @brief Handles the scanning procedure(s) when we have encountered a digit.
 * 
 * If we encounter a digit, other digits (or symbols) may follow after.
 * Thus, we account for this by peeking at the next [char]acter and converting
 * it to a digit. We add this to a total Integer by a simple base10 method.
 *
 * The lexeme itself is constructed from the peek chars in their raw form,
 * but only when the numeral is valid. An invalid numeral gets an INVALID
 * lexeme, though we could easily give it a "real" lexeme if preferred.
 *
 * @return Token A token representing the valid or invalid numeral found.
 */
Token Scanner::handleNumber()
{
    int v = 0;
    string lexeme = "";
    
    /** We peek ahead at the next character, and add it by base10 multiples until the next peek
     *  is no longer a digit/numeral.
     */
    do 
    {
      v = 10 * v + atoi(&peek);
      lexeme += peek;
    } while (readCharacter() && isdigit(peek));

    /** The numeral becomes a token only if it satisfies our lexicon. */
    Token *newToken;

    if(v >= NUMERAL_LOWER_BOUND && v <= NUMERAL_UPPER_BOUND)
    {
      newToken = new Token(NUMERAL, v, "num");
    }
    
    /** Otherwise, we create an error token and allow it to be handled as appropriate. */
    else
    {
      newToken = new Token(BADNUMERAL, v, spellOutTypeName(BADNUMERAL));
      //commented out -this is / was for debuggin.
      //cerr<<"Bad Numeral - Overflow / Underflow "<<endl;
    }
    return *newToken;
}

/**
 * @brief Function handles symbols read from the source PL program string.
 * 
 * We create a temporary token and a Type to reflect a symbol
 * that does not match ours.
 * We construct the token based off of the appropriate Type and
 * return it when completed.
 *
 * Certain symbols require special handling (dual symbols, such as
 * "->") and are accomodated for by peeking for a possible following
 * symbol. 
 * If none are found, we reverse the peek and continue.
 *
 * @see token.h
 * @see enum Type{};
 * @return Token A token that represents the symbol we handled.
 */
Token Scanner::handleSymbol()
{
    Token tempToken;
    Type newType = BADSYMBOL;

    switch (peek) 
    {
       case ';':
 	 newType = SC;
         tempToken = Token(newType, -1, ";");
	 break;
       case '.':
	 newType = PERIOD;
         tempToken = Token(newType, -1, ".");
	 break;
       case ',':
	 newType = COMMA;
         tempToken = Token(newType, -1, ",");
	 break;

       /** An '[' symbol can be followed by an ']' symbol. */
       case '[':
	 newType = LSB;
         tempToken = Token(newType, -1, "[");
	 //handling the dual symbol both square brackets "[]"
	 readCharacter();
	 if(peek == ']')
	 {
	   tempToken = Token(BOTHSQUAREBRACKETS, -1, "[]"); 
	 }
	 //we reverse the indexing operations that took place;
	 //a possible following symbol ']' was not found.
	 else
	 {
	   src_i--;
	 }
	 break;

       case ']':
	 newType = RSB;
         tempToken = Token(newType, -1, "]");
	 break;
       case '&':
	 newType = AND;
         tempToken = Token(newType, -1, "&");
	 break;
       case '|':
	 newType = PIPE;
         tempToken = Token(newType, -1, "|");
	 break;
       case '~':
	 newType = TILDA;
	 tempToken = Token(newType, -1, "~");
	 break;
       case '<':
	 newType = LT;
         tempToken = Token(newType, -1, "<");
	 break;
       case '=':
	 newType = EQUALS;
         tempToken = Token(newType, -1, "=");
	 break;
       case '>':
	 newType = GT;
         tempToken = Token(newType, -1, ">");
	 break;
       case '\\':
	 newType = FORSLASH;
         tempToken = Token(newType, -1, "\\");
	 break;
       case '+':
	 newType = PLUS;
         tempToken = Token(newType, -1, "+");
	 break;
	  
       /** An '-' symbol can be followed by an '>' symbol. */
       case '-':
         newType = MINUS;
         tempToken = Token(newType, -1, "-");
	 //handling the dual symbol arrow "->"
	 readCharacter();
	 if(peek == '>')
	 {
	   tempToken = Token(ARROW, -1, "->");
	 }
	 //we reverse the indexing operations that took place;
	 //a possible following symbol '>' was not found.
	 else
	 {
	   src_i--;
	 }	
	 break;

       case '*':
	 newType = TIMES;
	 tempToken = Token(newType, -1, "*");
	 break;
       case '/':
	 newType = DIVIDE;
         tempToken = Token(newType, -1, "/");
	 break;
       case '(':
	 newType = LB;
         tempToken = Token(newType, -1, "(");
	 break;
      case ')':
	 newType = RB;
         tempToken = Token(newType, -1, ")");
	 break;

      /** An ':' symbol can be followed by an '=' symbol. */
      case ':':
         newType = UNKNOWN;
         tempToken = Token(newType, -1, ":");
	 //handling the dual symbol Colon-Equals ":="
	 readCharacter();
	 if(peek == '=')
	 {
	   tempToken = Token(COLONEQUALS, -1, ":=");
	 }
	 //we reverse the indexing operations that took place;
	 //a possible following symbol '=' was not found.
	 else
	 {
	   src_i--;
	 }
         break; 
     }
     readCharacter();
     return tempToken;
}


/**
 * Handles the scanning duties for a regular character string.
 * Creates a string based off of whether the input line (until an atomic terminator) 
 * is a valid, full string.
 * A string is valid so long as it starts with an alphabetic character.
 * A valid alpha string can contain numbers and underscores.
 * It finally returns a token of a type determined by the characters read.
 * @return Token A type of token based on the kind of character symbol found.
 */
Token Scanner::handleCharString()
{
    //isalnum is an std function found in <cctype> and returns 0 if a char is not alphanumeric.
    string str = "";
    do 
    {
      str += peek;
    } while (readCharacter() && (isalnum(peek) || peek == '_'));
    
    
    //check if token's lexeme is too long
    if(str.size() > 80)
    {
      cerr<<"Bad Name - Overflow"<<endl;
      return Token (BADNAME, -1, str);
    }
    else
    {
      //Check to see if the token exists
      int existing_token = symTable->findLexeme(str);
      
      if (existing_token == -1) 
      {
	int index = symTable->makeEntry("ID","lexeme", str);
	return Token(IDENTIFIER, index, "name");
      } 
      else 
      {
	//TODO do we handle reserved words any differntly?
	if(existing_token < symTable->getNumOfReservedWords())
	{
	  //commented out - it was/is for debugging. useful :D for future? maybe :?
	  //cout<<"RESERVED WORD FOUND"<<endl;
	  return Token(IDENTIFIER, existing_token, str);
	  
	}
	return Token(IDENTIFIER, existing_token, "name");
      }
    }
}

/**
 * @brief Checks to see if a given input char is a special character.
 *
 * If the character is an accepted special character, it will trip one
 * of the following switch cases. Otherwise, it will hit the default
 * case and return false.
 * @param ch Input Character to compare.
 * @return boolean True if it's a special character.
 */
bool Scanner::isSpecial(char ch)
{
    // . < , = ; > [ + ] - & * | / ~ \ ( ) := [] ->
    switch (ch) 
    {
      case '.':
	return true;
	break;
      case '<':
	return true;
	break;
      case ',':
	return true;
	break;
      case '=':
	return true;
	break;
      case ';':
	return true;
	break;
      case '>':
	return true;
	break;
      case '[':
	return true;
	break;
      case '+':
	return true;
	break;
      case ']':
	return true;
	break;
      case '-':
	return true;
	break;
      case '&':
	return true;
	break;
      case '*':
	return true;
	break;
      case '|':
	return true;
	break;
      case '/':
	return true;
	break;
      case '~':
	return true;
	break;
      case '\\':
	return true;
	break;
      case '(':
	return true;
	break;
      case ')':
	return true;
	break;
      //adding in special case for the colonequals double sign ":="
      //see function HandleSymbol for more information
      case ':':
	return true;
	break;
      default:
	return false;
   }
}

/**
 * @brief Presently unused/for future use Scan function for the Scanner itelf.
 * This could be called from the Admin or Parser as needed.
 *
 * Main running function for the scanner, this returns tokens and then concatinates the token to a
 * string for output later.
 */
void Scanner::scan()
{
    Token temp;
    Token blank;

    do 
    {
      temp = nextToken();
      // std::cout << table->getAttributeWhere(temp.getValue(), "ID", "lexeme") << std::endl;
      tokenizedString += temp.toString();
    }
    while (inRange());
}

/**
 * Returns the tokenized string, in human readable form.
 * This function is for temporary use and debugging and will be dropped further 
 * into the project.
 * @return std::string
 */
string Scanner::getTokenizedString()
{
    return tokenizedString;
}

/**
 * Checks to see if the Index of the Scanner is within the input PL source file string size.
 * @see Admin::scan()
 * @return bool False if we are out of range.
 */
bool Scanner::inRange() 
{
    return src_i < (signed)source->size();
}
