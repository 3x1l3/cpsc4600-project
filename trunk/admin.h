#ifndef ADMIN_H
#define ADMIN_H

#include <vector>
#include <string>
#include <iostream>

class Admin {
private:
  std::vector<std::string> error_msgs;
  std::vector<int> error_line;
  
public:
  Admin();
  void recordError(std::string, int, int);

};


#endif