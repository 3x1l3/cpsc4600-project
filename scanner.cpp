<<<<<<< .mine
#include "scanner.h"
=======
#include token.cpp
#include number.cpp
>>>>>>> .r8

<<<<<<< .mine
=======

class Scanner {
	private:
	void SkipWhitespace();
	Token HandleNumber();
	int lineNum;
	int charNum;
	int sourceIndex;
	
	//lookahead character initially looking at first charcter in input source
	char ch;
	
	std::string source;
	std::vector tokens;
	
	public:
	Scanner();
	~Scanner() { };
	Token NextToken();
	void LoadSource(std::string);
	void RunScanner();
	void NextCharacter();
>>>>>>> .r8

Scanner::Scanner() {
    line = 0;

<<<<<<< .mine
    //Source index, per character
    src_i = 0;
    source = NULL;
    peek = ' ';
    assignment = Word();

=======
Scanner::Scanner() {
	lineNum = 0;
	charNum = 0;
	sourceIndex = 0;
	source = "";
>>>>>>> .r8
	
}

<<<<<<< .mine
/**
 * Load the source of the function, or rather the address of the source to conserve memory
 * @param src
 */
void Scanner::loadSource(std::string& src) {
    source = &src;
=======
void Scanner::LoadSource(std::string src) {
	source = src;
>>>>>>> .r8
	//set ch to the first item in the source
	ch = source[0];
	charNum = 1;
	sourceIndex = 0;
}

<<<<<<< .mine
/**
 * set the peek value to the next character if not out of bounds.
 */
void Scanner::readch() {
    if (src_i + 1 < source->size()) {
        peek = source->at(src_i+1);
        src_i++;
    }
=======
void Scanner::RunScanner()
{
  while(sourceIndex < source.size()) do
  {
   tokens.add( NextToken() );
   
   //add all tokens in the token vector to a hash table here??
    
    
  }
}
void Scanner::NextCharacter()
{
  sourceIndex++;
  ch = source[sourceIndex];
  charNum++;
}

    //skip all whitespace and newline until we hit a symbol
void Scanner::SkipWhitespace() 
{
	
    while(ch== ' ' || ch == '\t' || ch == '\n') do
    {
      if ( ch == ' ' )
      {
	NextCharacter();
      }
      else if (ch == '\t')
      {
	//may have problems with the charcter count on a line if a tab counts as more than one charcter in the text file.
	NextCharacter();
      }
      else if (ch == '\n') 
      {
	lineNum++;
	charNum = 0;
	NextCharacter();
	
      }
      else 
      {
	break;
      }
    }
		
	
>>>>>>> .r8
}

<<<<<<< .mine
/**
 * Overload of readch, checks to see if the next peeked character matches the parameter
 * @param c
 * @return bool
 */
bool Scanner::readch(char c) {
    readch();
    if (peek != c) return false;
    peek = ' ';
    return true;
=======
Token Scanner::NextToken() {	
	SkipWhitespace();
	
	Token tempToken = new Token();
	
	//if ch is a alphabetic char...
	//tempToken = HandleLetter();
	
	//if ch is a number...
	//tempToken = HandleNumber();
	
	//if ch is a symbol...
	//tempToken = HandleSymbol();
	
        //else...
	//mad error, something baad happened. or some unknown symbol was reached.
	
	return tempToken;
>>>>>>> .r8
}

<<<<<<< .mine
Token Scanner::nextToken() {

    //Skip Whitespaces

    for (src_i = 0; src_i < source->size(); src_i++) {
        peek = source->at(src_i);

        if ( peek == ' ' || peek == '\t') continue;
        else if (peek == '\n') line++;
        else
            break;

    }

    //Handle Special Symbols
/*
    switch (peek) {
    case ':':
        if (readch('=')) return assignment;
        else return Token(':');

    }
*/
    //Handle Digits
    if (isdigit(peek)) {
        int v = 0;
        do {
            v = 10 * v + atoi((char*)peek);
            src_i++;
            peek = source->at(src_i);
        } while ( isdigit(peek) );

    }

    return Token();
=======
//handleLetter and HandleSymbol will be similar to this
//NOTE there is psuedocode all over this function.  will not run.
Token Scanner::HandleNumber() 
{
  //create new token of type number
  Number tempNum = new Number(0);
  //create string to hold number.
  std::string holder = "";
  //add our current number to our holder string
  holder += ch;
  //call nextcharacter()
  NextCharacter();
  //start a loop
  while(ch != a number) do
  {
  //if ch is now still a number, add it to the string.
    if(ch == a number)
    {
      holder += ch;
      NextCharacter();
    }
    else
    {
      //do not call NextCharacter here becasue it is already looking at the next one 
      //that needs to be dealt with seperatly from this number.
      int value = holder.toInt();
      tempNum.SetValue(value);
    }
  }
  //return created token
  return tempNum;
  
// 	if (isdigit(ch)) 
// 	{
// 	int v = 0;
// 	do {
// 		v = 10 * v + atoi((char*)ch);
// 		sourceIndex++;
// 		ch = source[sourceIndex];
// 	} while ( isdigit(ch) );
// 	
// 	}
>>>>>>> .r8
}



