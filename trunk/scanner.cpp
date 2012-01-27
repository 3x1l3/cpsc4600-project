#include "scanner.h"

Scanner::Scanner(SymbolTable& table) {
    
    currentCharacter = 0;
    
    //Source index, per character
    src_i = 0;
    source = NULL;
    peek = ' ';
    symTable = &table; 
    

    string reservedWordsArray[] = {"begin", "end", "const", "array", "integer", "boolean", "proc", "skip", "read",
                                        "write", "call", "if", "do", "fi", "od", "false", "true"
                                       };


    reservedWords.insert(reservedWords.begin(), reservedWordsArray, reservedWordsArray+(sizeof(reservedWordsArray) / sizeof(std::string)));

}

/**
 * Load the source of the function, or rather the address of the source to conserve memory
 * @param src
 */
void Scanner::loadSource(string& src) {
    source = &src;
   
    //set ch to the first item in the source
    if (source->size() > 0 )
    {
        readCharacter();
    }

}


/**
 * set the peek value to the next character if not out of bounds.
 */
bool Scanner::readCharacter()
{
    if (inRange())
    {
        peek = source->at(src_i);
        src_i++;
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
 * Overload of readch, checks to see if the next peeked character matches the parameter
 * @param c
 * @return bool
 */
bool Scanner::readCharacter(char c) {
    readCharacter();
    if (peek != c) return false;
    peek = ' ';
    return true;
}

/**
 *
 */
Token Scanner::nextToken() {
    bool comment = false;

    //Skip Whitespaces
    do
    {
       
        if ( peek == ' ' || peek == '\t')
        {
            currentCharacter ++;
        }
        else if (peek == '\n')
        {
            currentCharacter = 0;
            comment = false;
        }
        else if (peek == '$')
        {
            cout<<"found comment"<<endl;
            currentCharacter ++; //TODO is this needed ?!?!?! :O
            comment = true;
        }
        else
        {
            if (comment == false)
            {
                break;
            }
        }
    } while (readCharacter() && ( peek == ' ' || peek == '\t' || peek == '\n' ||peek == '$' || comment == true));

    if (isSpecial(peek))
    {
      return handleSymbol();
    }

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


    cout<<peek<<endl;
    return Token(UNKNOWN, -1, "unknown character");
}

/**
 *
 */
Token Scanner::handleNumber()
{
    int v = 0;
    string lexeme = "";
    do {
        v = 10 * v + atoi(&peek);
	lexeme += peek;
    } while (readCharacter() && isdigit(peek));

    Token *newToken;
    if(v >= NUMERAL_LOWER_BOUND && v <= NUMERAL_UPPER_BOUND)
    {
      newToken = new Token(NUMERAL, v, lexeme);
    }
    else
    {
      newToken = new Token(BADNUMERAL, v, spellOutTypeName(BADNUMERAL));
      //TODO change the error handling to the admin class
      cerr<<"Bad Numeral - Overflow - "<<endl;
    }
    return *newToken;

}

/**
 * Function to handle symbols from the source string. Uses the symbol vector that
 * is defined in the constructor.
 * @return Token
 */
Token Scanner::handleSymbol()
{
   Token tempToken;
   Type newType = BADSYMBOL;
   switch (peek) {
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
      case '[':
	 newType = LSB;
         tempToken = Token(newType, -1, "[");
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
      case '-':
         newType = MINUS;
         tempToken = Token(newType, -1, "-");
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
   }
   readCharacter();
   return tempToken;
}
/**
 * Handle a regular character string. Though these strings must start with an alpha character, but can
 * have underscores and numbers.
 * @return Token
 */
Token Scanner::handleCharString()
{
    string str = "";
    do {
        str.append(&peek);
   
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
	return Token(IDENTIFIER, index, str);
      } 
      else 
      {
	//TODO do we handle reserved words any differntly?
	if(existing_token < symTable->getNumOfReservedWords())
	{
	    cout<<"RESERVED WORD FOUND"<<endl;
	}
	return Token(IDENTIFIER, existing_token, str);
      }
    }
}

bool Scanner::isSpecial(char ch)
{
 // . < , = ; > [ + ] - & * | / ~ \ ( ) := [] ->

   switch (ch) {
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
      default:
	 return false;
	 //TODO := [] -> double symbols
   }
}


/**
 * Main running function for the scanner, this returns tokens and then contatinates the token to a
 * string for output later.
 */
void Scanner::scan()
{
    Token temp;
    Token blank;
    do {
        temp = nextToken();
       // std::cout << table->getAttributeWhere(temp.getValue(), "ID", "lexeme") << std::endl;
        tokenizedString += temp.toString();
    }
    while (inRange());

}


/**
 * Return the tokenized string, in human readable form. This function will not be used once
 * we get further into the project.
 * @return std::string
 */
std::string Scanner::getTokenizedString()
{
    return tokenizedString;
}

/**
 * Range check to consolidate all the checks to see if src_i is below the size of the source
 * @return bool
 */
bool Scanner::inRange() {
    return src_i < source->size();
}




