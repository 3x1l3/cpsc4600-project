#include <string>
#include <ctype.h>
#include <iostream>
#include "token.h"

#include "scanner.h"
#include "symboltable.h"

using std::string;
using std::cout;
using std::endl;

int main() {

    SymbolTable* table = new SymbolTable();
    Scanner* scan = new Scanner(table);
    std::string source = "int;";
    scan->loadSource(source);
    scan->scan();
    cout << scan->getTokenizedString() << endl;
  
    return 0;

}
