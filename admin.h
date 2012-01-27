#ifndef ADMIN_H
#define ADMIN_H

#include <vector>
#include <string>
#include <iostream>
#include <sstream>

#include "scanner.h"
#include "symboltable.h"

#define MAX_ERRORS 10

using std::string;
using std::cout;
using std::endl;
class Scanner;
class Admin {
private:
  std::vector<string> error_msgs;
  std::vector<int> error_line;
  int current_line;
  int column;
  SymbolTable* table;
  Scanner* scanner;
  int symTIndex;
  bool correctLine;
  int errorCount;
  
public:
  Admin(string&);
  void recordError(string);
  int scan();
  void newLine();
  void increaseColumn();
  void resetColumn();

};


#endif