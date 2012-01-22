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
    std::string source = "word df g";
    scan->loadSource(source);
    Token first = scan->nextToken();
  
    cout << table->getAttributeWhere(0, "ID", "lexeme") << endl;
    return 0;

}
