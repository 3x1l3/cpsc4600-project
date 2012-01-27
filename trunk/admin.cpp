#include "admin.h"


Admin::Admin(string& src)
{
  current_line = 1;
  column = 1;
  symTIndex = 0;
  table = new SymbolTable();
  scanner = new Scanner(*table);
  correctLine = true;
  scanner->loadSource(src);
  
}

void Admin::recordError(string msg)
{
    correctLine = false;

  
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

void Admin::newLine()
{
  current_line++;
  correctLine = true;
}

void Admin::increaseColumn()
{
  column++;
}
void Admin::resetColumn()
{
  column = 1;
}


