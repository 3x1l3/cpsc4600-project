#include "scanner.h"

Scanner::Scanner(SymbolTable& table) {
    
    currentCharacter = 0;
    //Source index, per character
    src_i = 0;
    source = NULL;
    peek = ' ';
    symTable = &table; 
    

    char symbolarray[] = { '}', '{', '=', '+', '-', '/', ';', '*' };
    std::string symbolStrArray[] = { "CB", "OB", "EQ", "PLUS", "MINUS", "DIV", "SC", "MPLY" };
    std::string reservedWordsArray[] = {"begin", "end", "const", "array", "integer", "Boolean", "proc", "skip", "read",
                                        "write", "call", "if", "do", "fi", "od", "false", "true"
                                       };


    symbols.insert(symbols.begin(), symbolarray, symbolarray+sizeof(symbolarray));
    symbolStrings.insert(symbolStrings.begin(), symbolStrArray, symbolStrArray+(sizeof(symbolStrArray) / sizeof(std::string)));
    reservedWords.insert(reservedWords.begin(), reservedWordsArray, reservedWordsArray+(sizeof(reservedWordsArray) / sizeof(std::string)));

}

/**
 * Load the source of the function, or rather the address of the source to conserve memory
 * @param src
 */
void Scanner::loadSource(std::string& src) {
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

    if (ispunct(peek))
    {
        int index = checkSymbol(peek);
        if (index > -1) {
            readCharacter();
            return Token(symbolStrings.at(index), -1);
        } else {
	    //Record Error Here.
	    //log->recordError("Invalid character found: ", line, col);
	    readCharacter();
	}
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



    return Token("UNKNOWN" -20);
}

/**
 *
 */
Token Scanner::handleNumber()
{
    bool isNum = true;
    int v = 0;
    do {
        v = 10 * v + atoi(&peek);



        if (isdigit(peek) )
        {
            isNum = true;
        }
        else
        {
            isNum = false;
            break;
        }
    } while (isNum == true && readCharacter());

    Token *newToken = new Token("NUM", v);
    return *newToken;

}

/**
 * Function to handle symbols from the source string. Uses the symbol vector that
 * is defined in the constructor.
 *
 * TODO: there needs to be only one return from this function. Still have to figure out how to do that
 *
 * @return Token
 */
Token Scanner::handleSymbol()
{
   
    int index = checkSymbol(peek);
    if (index > -1) {
        readCharacter();
        return Token(symbolStrings.at(index), -1);
    }

    return Token("UNKNOWN", -2);
}
/**
 * Handle a regular character string. Though these strings must start with an alpha character, but can
 * have underscores and numbers.
 *
 * TODO: we need to check for reserved words in here.
 *
 * @return Token
 */
Token Scanner::handleCharString()
{
    std::string str = "";
    do {
        str.append(&peek);
       
    } while (readCharacter() && (isalnum(peek) || peek == '_'));


    int index = symTable->makeEntry("ID","lexeme", str);

    return Token("ID", index);
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

/**
 * Lets check to see if the passed symbol exists in our symbol vector.This will return the index
 * if it is found so > 0 and -1 if it doesn't find one
 * @return int
 */
int Scanner::checkSymbol(char symbol) {

    for (int i = 0; i < symbols.size(); i++) {
        if (symbols.at(i) == symbol)
            return i;

    }
    return -1;
}

