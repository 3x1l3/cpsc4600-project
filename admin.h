#ifndef ADMIN_H
#define ADMIN_H

#include <vector>
#include <string>
#include <iostream>

#include "scanner.h"
#include "symboltable.h"

using std::string;
using std::cout;
using std::endl;

class Admin {
private:
  std::vector<string> error_msgs;
  std::vector<int> error_line;
  int current_line;
  int column;
  SymbolTable* table;
  Scanner* scanner;
  int symTIndex;
  
public:
  Admin(string&);
  void recordError(string, int, int);
  void scan();

};


#endif