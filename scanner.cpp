#include "scanner.h"

Scanner::Scanner() {
    line = 0;
    //Source index, per character
    src_i = -1;
    source = NULL;
    peek = ' ';
 
}

/**
 * Load the source of the function, or rather the address of the source to conserve memory
 * @param src
 */
void Scanner::loadSource(std::string& src) {
    source = &src;
    //set ch to the first item in the source
    if((source->size()-1) > 0 )
    {
      readCharacter();
    }

}


/**
 * set the peek value to the next character if not out of bounds.
 */
void Scanner::readCharacter() {
    if (src_i + 1 < (source->size()-1)) {
        peek = source->at(src_i+1);
        src_i++;
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

Token Scanner::nextToken() {
  
    //Skip Whitespaces
      do
      {
        if ( peek == ' ' || peek == '\t')
	{
	  readCharacter();
	  continue;
	}
        else if (peek == '\n') 
	{ 
	  readCharacter();
	  line++; 
	}
        else
	{
	  break;
	}
      }while(peek == ' ' || peek == '\t' || peek == '\n') ;

    //Handle Special Symbols
/*
    switch (peek) 
    {
    case ':':
        if (readch('=')) return assignment;
        else return Token(':');

    }
*/
  
    //Handle Digits
    if (isdigit(peek)) 
    {
      std::cout<<"isdigit"<<std::endl;
      handleNumber();
   
    }
//         int v = 0;
//         do {
//             v = 10 * v + atoi(&peek);
//             src_i++;
//             readCharacter(); 
//         } while (isdigit(peek) && src_i <= (source->size()-1));//peek == '0' || peek == '1' || peek == '2' || peek == '3' || peek == '4' || peek == '5' || peek == '6'|| peek == '7'|| peek == '8'|| peek == '9' );
// 	std::cout<<"after isdigit"<<std::endl;
// 	
// 	//instead of pasing in v to the token, we might have to pass in a index to the symbol table
// 	//but im not sure how to create an entry into the symbol table right now.
// 	//but this is where we add our tokens attributes to the symbol table, and v would be the index of
// 	//this token.
// 	Token *newToken = new Token("NUM", v);
// 	return *newToken;
    //}

    return Token();
}
Token Scanner::handleNumber()
{
  int v = 0;
  do {
    v = 10 * v + atoi(&peek);
    src_i++;
    readCharacter(); 
  } while (isdigit(peek) && src_i <= (source->size()-1));//peek == '0' || peek == '1' || peek == '2' || peek == '3' || peek == '4' || peek == '5' || peek == '6'|| peek == '7'|| peek == '8'|| peek == '9' );

  return Token();
  
}
Token Scanner::handleSymbol()
{
  return Token();
}
Token Scanner::handleCharString()
{
  return Token();
}


