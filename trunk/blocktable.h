#ifndef BLOCKTABLE_H
#define BLOCKTABLE_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include "symboltable.h"

using namespace std;

//constant table size limit
#define MAXBLOCK 10
enum Kind { CONSTANT, VARIABLE, ARRAY, PROCEDURE, DEFINED };
//added in CINTEGER and CBOOLEAN to represent constant variable types
enum mType { INTEGER, BOOLEAN, UNIVERSAL, CINTEGER, CBOOLEAN };

typedef struct {

	 //integer code for ID
	int id;
	
	//Kind is an enum type to represent objects Kind e.g. CONSTANT VARIABLE ARRAY PROCEDURE UNDEFINED
	Kind okind;
	
	//mType is an enum type to represent datatypes in PL e.g. Integer Boolean Universal ConstantInteger, ConstantBoolean
	mType otype;
	
	//Only for array types this entry will be filled. All others will have zero
	int size;
	
	//Only for constants
	int value;	
	
} TableEntry; //Extention of the token object?



class BlockTable {
	public:
		BlockTable(SymbolTable&);
		~BlockTable();
		
		//Returns true when in the current block, false otherwise
		bool search(int lookFor);
		bool search(int lookFor, TableEntry& entry);
		bool search(int , int&);
		
		//Return true when the object is stored successfully. returns false if the current block contains the object already
		bool define(int newid, Kind newkind, mType newtype, int newsize, int newvalue);
		bool define(int newid, Kind newkind, mType newtype);
		
		//I'm going to say to redefine a size of an entry in a block table it must exist in the current block already.
		bool redefineSize(int, int);
		
		//looks through ALL BLOCKs for lookfor variable.  if found, error is true, else error is false.
		//returns tableentry where look for was found, and if not found, returns an uninitialized TableEntry
		TableEntry find(int lookfor, bool& error);
		
		//returns true if a new block table entry is created, returns false otherwise
		bool newBlock(); 
		
		//Decreaments the current block index. So it decrements our index and pops a block off the block table.
		bool endBlock();
		
		//Print the passed in block to screen in a tabular format
		void printBlock(vector<TableEntry* > );
		
		//Overload to print out a different header title for each block
		void printBlock(vector<TableEntry* >, string);
		
		//Print all blocks using the printBlock method defined
		void printAllBlocks();
		
		//Return current block
		vector<TableEntry* > returnCurrentBlock();
		
		//Return all blocks
		vector< vector<TableEntry* > > returnAllBlocks();
		
		//Format a row for the printBlock function and testing purposes of find
		string tableEntrytoString(TableEntry);
		
		SymbolTable* table;
		
	//Helper function for formatting kind enum
	string convertKind(int);
	
	//Helper function for formatting type enum
	string convertType(int);
		
	private:
	
	//All Blocks defined in a vector
	vector< vector<TableEntry* > > blocks;
	
	//Block defined as the current block. This block has not been placed at the end of the vector yet.
	vector<TableEntry*> currentBlock;
	
	//Counter to keep track of the number of blocks inside the vector
	int currentBlockIndex;
	

	
	//
	
	
	
};

#endif