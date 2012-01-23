#include <string>
#include <ctype.h>
#include <iostream>
#include "token.h"
#include <fstream>
#include <istream>
#include <ostream>

#include "scanner.h"
#include "symboltable.h"

using std::string;
using std::cout;
using std::endl;

int main() {

    SymbolTable* table = new SymbolTable();
    Scanner* scan = new Scanner(table);
    //////////////////////////////////////////
    
    string line;
    string file;
    std::ifstream myfile ("testTextFile.txt");
    if (myfile.is_open())
    {
      while ( myfile.good() )
      {
	getline (myfile,line);
	cout << line << endl;
	line+='\n';
	file += line;
      }
      myfile.close();
    }
    else cout << "Unable to open file"; 
    
    myfile.close();
    
    ///////////////////////////////////////
    std::string source = file;//"$comment line \n int x = 101; ";
    scan->loadSource(source);
    scan->scan();
    cout << scan->getTokenizedString() << endl;
  
    return 0;

}
