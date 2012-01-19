#include <string>
#include <ctype.h>
#include <iostream>
#include "token.h"

#include "scanner.h"


using std::string;
using std::cout;
using std::endl;

int main() {

    Scanner* scan = new Scanner();
    std::string source = "234 57623 2342342";
    scan->loadSource(source);
    Token first = scan->nextToken();
    cout << first.getValue() << endl;
    return 0;

}
