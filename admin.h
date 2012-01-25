#ifndef ADMIN_H
#define ADMIN_H

#include <vector>
#include <string>
#include <iostream>

#include "scanner.h"
#include "symboltable.h"

class Admin {
private:
  std::vector<std::string> error_msgs;
  std::vector<int> error_line;
  int current_line;
  int column;
  SymbolTable* table;
  Scanner* scanner;
  
public:
  Admin(std::string&);
  void recordError(std::string, int, int);
  void scan();

};


#endif