#include <string>
#include <ctype.h>


#include <iostream>
#include "token.h"
#include "word.h"
#include "scanner.h"
#include "number.h"




using std::string;
using std::cout;
using std::endl;

int main() {

    Scanner* scan = new Scanner();
    std::string source = "This is a new source";
    scan->loadSource(source);
    scan->nextToken();
    return 0;




}
