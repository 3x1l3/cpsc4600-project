#define NUM 256
#define ID 257
#define TRUE 258
#define FALSE 259

#include <string>
#include <ctype.h>
#include <stdlib.h>
#include <iostream>
#include "token.cpp"
#include "scanner.cpp"
#include "number.cpp"





int main() {
	
	Scanner* scan = new Scanner();
	scan->loadSource("This is a source");
	scan->nextToken();
	return 0;
	
}