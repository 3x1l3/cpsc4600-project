/**
 * @brief The main file for running the Interpreter seperately.
 * 
 * @file Interpreter.cpp
 * 
 * This is a standalone version of the interpreter.
 * It impliments the same class, but allow you to compile and run
 * a separate Interpreter for any given .itp file.
 * 
 * By default, it uses interpInput.itp.
 * 
 * It takes two arguments - the name of the interp file,
 * and the "Step" flag of -s.
 * 
 * Step is used for debugging and stepping through the interp
 * functions by the interpreter.
 * 
 * 
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date April 17th, 2012
 */

#include <iostream>
#include <string>
#include "interp.h"

using std::cout;
using std::cin;
using std::string;

/** Main and only function. Check above for flags. */
int main( int argc, char* argv[])
{
  string interpSourceFile;
  bool step = false;
  
  //Deal with our arguments
  if(argc == 1)
  {
    cout << "\nYou did not specify an input file. Using \"interpInput.itp\"." << endl;
    interpSourceFile = "interpInput.itp";
  }
  else
  {
    //if we have multiple inputs
    if(argc > 2)
    {
      string stepVar = argv[2];
      //if our flag is correct
      if(stepVar == "-s")
      {
	step = true;
      }
    }
    //set out interp file string.
    interpSourceFile = "";
    interpSourceFile.append(argv[1]);
  }
  
  cout << "\nRunning interpreter for file: " << interpSourceFile << endl;
  Interpreter* interp = new Interpreter(interpSourceFile, step);
  
  cout << "\nStandalone Interpreter has finished running." << endl;
  delete interp;
  return 0;
}