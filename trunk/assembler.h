// Interface to the assembler class.



#ifndef ASSEMBLER_H
#define ASSEMBLER_H

using namespace std;
#include <iostream>
#include <string>
#include <sstream>

using std::stringstream;
//#include "parser.h"

const int MAXLABEL = 500;

class Assembler
{
  public:
   Assembler(istream &in, ostream &out); 
   ~Assembler();
   // The two passes of the assembler.
   void firstPass(); 
   void secondPass(string alpha);

  private:
   int labelTable[MAXLABEL]; 
   int currentAddress; 
   istream *insource;  // Input file
   ostream *outsource; // Output file 
};
#endif
