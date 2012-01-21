#include <string>
#include <ctype.h>
#include <iostream>
#include "token.h"

#include "scanner.h"


using std::string;
using std::cout;
using std::endl;

int main() {

  cout<<"herer"<<endl;
    Scanner* scan = new Scanner();
    std::string source = "123 45678 9";
    scan->loadSource(source);
    Token first = scan->nextToken();
    cout << first.getValue() << endl;
    cout<< first.getName() <<endl;
    
    first = scan->nextToken();
    cout << first.getValue() << endl;
    cout<< first.getName() <<endl;
    
    first = scan->nextToken();
    cout << first.getValue() << endl;
    cout<< first.getName() <<endl;
    return 0;

}
