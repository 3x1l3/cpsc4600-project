#include <string>
#include <ctype.h>
#include <iostream>
#include "token.h"
#include <fstream>
#include <istream>
#include <ostream>

#include "admin.h"


using std::string;
using std::cout;
using std::endl;

	   
int main() {

    
    //////////////////////////////////////////
    
    string line;
    string file;
    std::ifstream myfile ("testTextFile.txt");
    if (myfile.is_open())
    {
      while ( myfile.good() )
      {
	getline (myfile,line);
	line+='\n';
	file += line;
      }
      myfile.close();
    }
    else cout << "Unable to open file"; 
    
    myfile.close();
    
    ///////////////////////////////////////
   
    Admin* admin = new Admin(file);
    admin->scan();
  
    return 0;

}
