#include "admin.h"


Admin::Admin(string& src)
{
  current_line = 1;
  column = 1;
  symTIndex = 0;
  table = new SymbolTable();
  scanner = new Scanner(*table);
  
  scanner->loadSource(src);
  
}

void Admin::recordError(string msg, int line, int col)
{

    cerr << msg << " Line: " << line << " Col: " << col << endl;
    error_line.push_back(line);
    error_msgs.push_back(msg);
  
}	

void Admin::scan()
{
  Token tok;
  do { 
    tok = scanner->nextToken();
   cout << tok.toString() << endl;
  }
  while(scanner->inRange()) ;

}


