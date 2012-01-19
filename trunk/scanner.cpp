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

}


/**
 * set the peek value to the next character if not out of bounds.
 */
void Scanner::readch() {
    if (src_i + 1 < source->size()) {
        peek = source->at(src_i+1);
        src_i++;
    }
}




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
}

Token Scanner::nextToken() {
  
    //Skip Whitespaces
    for (src_i = 0; src_i < source->size(); src_i++) {
	
	peek = source->at(src_i);
	
        if ( peek == ' ' || peek == '\t') continue;
        else if (peek == '\n') { line++; }
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
            v = 10 * v + atoi(&peek);
            src_i++;
            peek = source->at(src_i);   
        } while ( isdigit(peek) );
	
      return Token(NUM);
    }

    return Token();
}



