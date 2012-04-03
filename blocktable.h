/**
 * @brief The specification for the Admin Class Object
 *
 * @file blocktable.h
 * 
 * The BlockTable class is used to manage a series and collection
 * of Blocks, which are themselves a collection of TableEntry
 * structure records that refer to a specific "entry" in a given
 * segment of PL code.
 *
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date March 1st to April 2nd, 2012
 */

#ifndef BLOCKTABLE_H
#define BLOCKTABLE_H

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>

/** We include the symbol table to access the direct token information. */
#include "symboltable.h"

using namespace std;

/** A constant defining the size limit of our table. */
#define MAXBLOCK 10

/** Enumerates the KIND an object referred to by a TableEntry */
enum Kind { CONSTANT, VARIABLE, ARRAY, PROCEDURE, DEFINED };

/** The TYPE of TableEntry; The PL Grammar features the first three.
 * CInteger and CBoolean reflect Constant Integer and Constant Boolean. 
 */
enum mType { INTEGER, BOOLEAN, UNIVERSAL, CINTEGER, CBOOLEAN };


/**
 * @brief A special data structure used to define a given PL code object.
 *
 * A TableEntry is a kind of record that amalgamates a given Entry in a given Block.
 * These "block entries" are a kind of programming object - like an Array, or an 
 * INTEGER, and this structure combines all of the relevent tokens and their information
 * into one specific record.
 *
 * Which is to say, for a given variable X, we can know that X is an integer, has value Y,
 * and has the Table's referential ID of #.
 *
 */
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
	
} TableEntry; //Logical "Extension" of a Token's consolidated representation


/**
 * @brief The structure facilitating the creation and manipulation of Program Blocks.
 *
 * The BlockTable data structure allows us to keep track of Program Blocks, through
 * a vector of such objects. A block itself is a Vector of TableEntry structures,
 * and represents a logical block in a PL program, defined by a Begin/End pair such
 * as a procedure call.
 *
 * The blocktable provides the capabilities and capacities for adding new blocks,
 * searching for entries in given blocks, adding entries to blocks and smartly
 * allowing us to construct these TableEntry structures through limited and properly
 * defined access to the core compiler's Symbol Table.
 *
 */
class BlockTable {
	
  public:
	  
    BlockTable(SymbolTable&);
    ~BlockTable();
    
    // The following is used by Find() to check if an entry is in ANY of the above blocks.
    // They return True if a symbol is in the current block, false otherwise.
    
    /** Checks if a given TableEntry ID is found in the current Block. @see find()*/
    bool search(int lookFor);
    
    /** Locates a given ID in the current Block, then links said entry to the input TableEntry reference. */
    bool search(int lookFor, TableEntry& entry);
    
    /** Locates a given TE ID in the current Block, and pairs its index with the input integer reference. */
    bool search(int , int&);
    
    /** 
     * Creates an object if we don't find it in the current block.
     * We don't call Find as the caller of Define will know whether to check 
     * the other blocks.
     * Return true when the object is stored successfully. returns false if the current block contains the object already
     */
    bool define(int newid, Kind newkind, mType newtype, int newsize, int newvalue);
    
    /** Defines a new block entry, but for non-const/non-array types. */
    bool define(int newid, Kind newkind, mType newtype);
    
    /** Redefines the size of an Array entry in the current block, if it is found, otherwise returns false. */
    bool redefineSize(int, int);
    
    //looks through ALL BLOCKs for lookfor variable.  if found, error is true, else error is false.
    //returns tableentry where look for was found, and if not found, returns an uninitialized TableEntry
    /** 
     * Searches through all of the blocks for a corresponding tableEntry and returns it.
     * Otherwise, it returns a blank entry and an activated (TRUE) error flag by reference. 
     */
    TableEntry find(int lookfor, bool& error);
    
    /** Push our old block onto the Table and create a new one for the new scope. Returns true-success/false-failure. */
    bool newBlock(); 
    
    /** Decreaments the current block index, pops a block off the block table, and replaces the currentBlock with the previous one. */
    bool endBlock();
    
    /** Prints the passed-in Block (tableentry vector) in a tabular format. */
    void printBlock(vector<TableEntry* > );
    
    /** Prints all of the table entries in the passed-in block, but with a manual heading. */
    void printBlock(vector<TableEntry* >, string);
    
    /** Prints all of the Block data in our Blocks vector using the above methods. */
    void printAllBlocks();
    
    /** Returns a string representation of a Kind enum. */
    string convertKind(int);
    
    /** Returns a string representation of a Type enum. */
    string convertType(int);

    /** Returns a string representation of a Table Entry. */
    string tableEntrytoString(TableEntry);
    
    
    /** Returns the current block (TableEntry* vector) */
    vector<TableEntry* > returnCurrentBlock();
    
    /** Returns every block except the current one, since it is directly accessible. */
    vector< vector<TableEntry* > > returnAllBlocks();
    
    /** Empty pointer, which has a Scanner's Symbol Table attached when this class is Constructed.*/
    SymbolTable* table;
    
    
  private:
    
    /**
     * A vector of Blocks.
     * This represents a "table" of records, since a Block is a vector of table entries.
     */
    vector< vector<TableEntry* > > blocks;
    
    /** 
     * The current scope/block.
     * All operations are done on this block. When we enter a new/deeper Scope,
     * we push this block onto our main vector.
     * Likewise, when the current scope dies, we do the inverse, and refill this block.
     */
    vector<TableEntry*> currentBlock;
    
    //Counter to keep track of the number of blocks inside the vector
    /** Counter that keeps track of our current nested level/the number of blocks within the vector. */
    int currentBlockIndex;
	
	
};

#endif