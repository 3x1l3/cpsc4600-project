#include "admin.h"


Admin::Admin()
{
  
}

void Admin::recordError(std::string msg, int line, int col)
{

    std::cout << msg << " Line: " << line << " Col: " << col << std::endl;
    error_line.push_back(line);
    error_msgs.push_back(msg);
  
}

