#include "admin.h"


Admin::Admin(std::string& src)
{
  current_line = 1;
  column = 1;
  scanner = new Scanner();
  table = new SymbolTable();
  scanner->loadSource(src);
}

void Admin::recordError(std::string msg, int line, int col)
{

    std::cout << msg << " Line: " << line << " Col: " << col << std::endl;
    error_line.push_back(line);
    error_msgs.push_back(msg);
  
}	

void Admin::scan()
{
  Token tok;
  do { 
    tok = scanner->nextToken();
   std::cout << tok.toString() << std::endl;
  }
  while(scanner->inRange()) ;

}


