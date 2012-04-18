/**
 * @brief A file for testing completed ASM files with the Assembler.
 * 
 * Takes an input .asm file and assembles it,
 * sending the completed file to a test.itp file.
 */

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include "Assembler.h"

using std::stringstream;
using std::cout;
using std::cin;
using std::string;

int main(int argc, char* argv[])
{
  
  string ASMSourceFile;
  
  //Deal with our arguments
  if(argc == 1)
  {
    cout << "\nYou did not specify an input file. Using \"test2.asm\"." << endl;
    ASMSourceFile = "test2.asm";
  }
  else
  {
    //set out interp file string.
    ASMSourceFile = "";
    ASMSourceFile.append(argv[1]);
  }
  
  std::ifstream asmStringFile;
  asmStringFile.open(ASMSourceFile.c_str());
  string inputString;
  string temp;
  while(asmStringFile.good())
  {/*
    asmStringFile >> temp;
    inputString.append(temp.append("\n"));
    cout << "ASM OUTPUT \t" << temp << endl;*/
    getline(asmStringFile, temp);
    temp += '\n';
    inputString += temp;
  }
  asmStringFile.close();
  cout << "Main input string:\n\n" << inputString << endl;
  
  std::ifstream asmFile, asmFile2;
  asmFile.open(ASMSourceFile.c_str());
  //This will be the file read by our Interpreter.
  std::ofstream interpCode("interpInput.itp");
  
  
  Assembler* myASM = new Assembler( asmFile, interpCode);
  //Our actual Assembler passes.
  myASM->firstPass();
  

  cerr << "\n\n\nOutputting asdasdasd\n" << inputString << endl;
  //Hossain's Assembler does not reset his stream. So we cheat.
  myASM->secondPass(inputString);
  
  cout << "Assembly Complete.\n";
  
  return 0;
}