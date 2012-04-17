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
#include "Assembler.h"
#include "interp.h"

/** Namespaces to prevent ugly code */
using std::string;
using std::cout;
using std::endl;

/** Function prototypes. */
void runAssembler(Admin* admin);
void runInterpreter(bool step);

/** Argc and Argv for later expansion. */
int main( int argc, char* argv[]) 
{

    /**	
     * Read the file to a stream, then send each line to a String.
     * We append our own newline and attach this to our whole File string.
     * Repeat until we reach the end of the file.
     * This might have scaling issues.
     */

    string line;
    string file;
    
    /* //DISPLAY ALL INPUT STRINGS
      // [1] IS THE ACTUAL FIRST PARAMETER! [0] IS THE BINARY NAME!
    if(argc > 0)
      for(int i = 0; i < argc; i++)
      {
	cout << "\n\nMAXIMUM TEST " <<  argv[i] << "\n\n" << endl;
      }
  */
    
    /**
     * We check to see if there was an input parameter, and use that as the filename
     * We do NOT accept multiple inputs (yet?).
     * Choose the first one if multiple detected. If none, then ask for a file name.
     */
    string inputFileString = "";
    if(argc == 1)
    {
      cout << "\nYou did not specify an input file. Please type the name of a file in this local/root folder: " << endl;
      cin >> inputFileString;
    }
    else
    {
      if(argc > 2)
	cout << "\nMultiple inputs detected. Using the first input: " << argv[1] << " ONLY." << endl;
      
      inputFileString.append(argv[1]);
    }
    cout << endl << "\nAttempting to scan Filename: " << inputFileString << endl;
    std::ifstream myfile (inputFileString.c_str());
  
    
    
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
    {
      cout << "Unable to open file: " << inputFileString << " . Please recheck your filename and try again." << endl;
      return 0;
    }
    
    myfile.close();
    
    /**
     * We create our administration object with our file string.
     * It then begins the Scanning process.
     * We could also have some error handling here, but that is not required.
     * @see Admin()
     */
    Admin* admin = new Admin(file);
    int status = admin->scan();

    /**
     * 
     * Assembler Part
     * This is where our assembler is called.
     */
    //if our program scanned and parsed successfully
    if (status == 0)
    {
      //Our scanning was successful.
      cout << "Scanning successful" << endl;
      
      cout << "\nThe Parsed Output is now going to be passed to the Assembler...Input a char to continue.\n";
      char throwAway;
      cin >> throwAway;
      runAssembler(admin);
      
      cout << "\nWould you like to run the interpreter? (y / n) ";
      cin >> throwAway;
      
      if(throwAway == 'y')
      {
	cout << "\nReady to run Interpreter. Would you like the interpreter to Step through the program? (y / n)" << endl;
	cin >> throwAway;
	if(throwAway == 'y')
	  runInterpreter(true);
	else
	  runInterpreter(false);
      }
      
    }
    else
      cout << "Program contains scan errors" << endl;

    cout << "\nPL Compiler Complete." << endl;
    return 0;
}


/**
 * @brief Operates the Assembler component of the compiler.
 * 
 * Use this function with a corresponding completed Parser
 * code in order to run the Assembler on it.
 * The code is then sent to interpInput.itp
 */
void runAssembler(Admin* admin)
{
  //Setup our base streams for the Assembler to use
  std::ifstream asmFile;
  asmFile.open("test.asm");
  
  //This will be the file read by our Interpreter.
  std::ofstream interpCode("interpInput.itp");
  
  Assembler* myASM = new Assembler( asmFile, interpCode);
  
  //Our actual Assembler passes.
  myASM->firstPass();
  //Hossain's Assembler does not reset his stream. So we cheat.
  myASM->secondPass(admin->getASM());
  
  cout << "Assembly Complete.\n";
}

/**
 * @brief Main function to run our Interpreter.
 * 
 * Use this function to run the Interpreter on assembled PL code.
 * This assumes that the assembled PL interpreter code is 
 * present in a file called interpInput.itp.
 */
void runInterpreter(bool step)
{
  cout << "\nRunning interpreter..." << endl;
  Interpreter* interp = new Interpreter("interpInput.itp", step);
}