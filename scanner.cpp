#include "scanner.h"

Scanner::Scanner() {
    line = 0;
    //Source index, per character
    src_i = 0;
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
    if((source->size()) > 0 )
    {
      readCharacter();
    }

}


/**
 * set the peek value to the next character if not out of bounds.
 */
void Scanner::readCharacter() 
{
    if(src_i == 0)
    {
      peek = source->at(0);
      src_i ++;
    }
    else if ((src_i) < (source->size())) {
        peek = source->at(src_i);
        src_i++;
    }
    else
    {
      std::cout<<"out of bounds read"<<std::endl;
      std::cout<<"peek = " <<peek<<std::endl;
      std::cout<<"src_i = "<<src_i<<std::endl;
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
	  std::cout<<"peek in whitespace1 = "<<peek<<std::endl;
	  std::cout<<"whitespace1"<<std::endl;
	  readCharacter();
	  continue;
	}
        else if (peek == '\n') 
	{ 
	  std::cout<<"whitespace2"<<std::endl;
	  readCharacter();
	  line++; 
	}
        else
	{
	  std::cout<<"whitespace3"<<std::endl;
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
      Token temp = handleNumber();
      return temp;
   
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
  bool isNum = true;
  int v = 0;
  do {
    v = 10 * v + atoi(&peek);
    
    readCharacter();
       
    if(isdigit(peek) )
    {
      isNum = true;
    }
    else
    {
      isNum = false;
      break;
    }
  } while (isNum == true && src_i < (source->size()-1));
  
  Token *newToken = new Token("NUM", v);
  return *newToken;
  
}
Token Scanner::handleSymbol()
{
  return Token();
}
Token Scanner::handleCharString()
{
  return Token();
}