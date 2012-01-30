/**
 * @brief The main "runner" file for the overall Compiler Project.
 *
 * @file main.cpp
 *
 * Currently, we only employ a Scanner which constructs a Symbol 
 * Table.
 * This is accomplished through an included Administration class.
 *
 *	We use a standard set of files and an Administration and Token
 * Classes to run the base Scanner.
 *
 * We have a static text file containing our sample PL code in the
 * Root directory. This code is then sent into a string which we 
 * forward to our constructors for the scanner and admin.
 * 
 * We use local file for now to speed up testing and debugging.
 * At a later date, it will read it from the argc and argv vars.
 *
 * This file has no associated header.
 *
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to 31st, 2011
 **/

/** Standard C and C++ Libraries */
#include <string>
#include <ctype.h>
#include <iostream>
#include <fstream>
#include <istream>
#include <ostream>

/** Associated custom libraries/Classes */
#include "admin.h"
#include "token.h"

/** Namespaces to prevent ugly code */
using std::string;
using std::cout;
using std::endl;

/** Argc and Argv for later expansion. */
int main( int argc, char* argv[]) {

    
	/**	
	 * Read the file to a stream, then send each line to a String.
	 * We append our own newline and attach this to our whole File string.
	 * Repeat until we reach the end of the file.
	 * This might have scaling issues.
	 */

    string line;
    string file;
    std::ifstream myfile ("testTextFile.txt");
  
	if ( myfile.is_open() )
    {
      while ( myfile.good() )
      {
		getline (myfile, line);
		line += '\n';
		file += line;
      }
      myfile.close();
    }
	/** We could incorporate an error class here, but for now, it is not necessary as this is not a compiler operation */
    else 
		cout << "Unable to open file"<<endl;
    
    myfile.close();
    
   
	/**
	 * We create our administration object with our file string.
	 * It then begins the Scanning process.
	 * We could also have some error handling here, but that is not required.
	 * @see Admin()
	 */
    Admin* admin = new Admin(file);
    int status = admin->scan();

    if (status == 0)
	   cout << "Scanning successful" << endl;
    else
       cout << "Program contains scan errors" << endl;

    return 0;

}