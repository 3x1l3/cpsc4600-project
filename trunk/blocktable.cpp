/**
 * @brief The definitions for the Admin Class Object
 *
 * @file blocktable.cpp
 * 
 * The BlockTable class manages a series and collection of Blocks.
 * A Block is a collection of TableEntry records, which detail a specific
 * semantic object - such as "X", which is an Integer of (value) 5.
 *
 * The ancilliary functions allow us to keep track of the current variables
 * and properly check for scope issues and problems by checking for
 * any previous declarations of a current object, and checking for 
 * consistency (i.e. that an Integer operation is being applied to an int).
 *
 * This is accomplished at parse-time by constructing and verifying TableEntry 
 * information through a single-access to the Scanner's symbol table.
 *
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date March 1st to April 2nd, 2012
 */

/** Specification File. */
#include "blocktable.h"


//---------------------------------------------------------------------------------------------
/** 
 * @brief Base Constructor.
 *
 * A BlockTable can only be constructed if a Scanner, and thus a SymbolTable, exist.
 * We assign the address of the symbol table to our local pointer, and set the base 
 * index of our block level/scope for the table/program.
 */
BlockTable::BlockTable(SymbolTable& symbolTable) 
{
  currentBlockIndex = 0;
  table = &symbolTable;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
/**
 * @brief Simple search function. 
 *
 * The user does not need the reference, but just a confirmation
 * that a given TableEntry ID exists in our BlockTable.
 * 
 * The TableEntry refrence is discarded automatically.
 * @param lookfor
 * @return bool a boolean of if the ID was found
 */
bool BlockTable::search(int lookFor)
{
  TableEntry tbl;
  return search(lookFor, tbl);  
}
//---------------------------------------------------------------------------------------------
/** 
 * @brief Search function with ID and actual Entry returns.
 * 
 * Checks all of the entries in the current block to see if the
 * entry ID is found.
 * If found, we return true, but also attach the given item's address
 * to the passed-in reference parameter.
 * Thus, we return a reference to the item as well as the TableEntry ID.
 * 
 */
bool BlockTable::search(int lookFor, TableEntry& entry) 
{
  for (int i = 0; i < (int)currentBlock.size(); i++) 
  {
    
    if (currentBlock.at(i)->id == lookFor) 
    {
      entry = *currentBlock.at(i);
      
      return true;
    }
    
  }
  return false;
}

/**
 * @brief Searches for a given entry and also returns its index.
 *
 * Determines (boolean) if a given lookfor (TE ID) is in the
 * current block. If not, it returns a false. If it is found, it will
 * return true as well as attaching the given Block index (location of
 * the item) to the integer reference.
 *
 */
bool BlockTable::search(int lookFor, int& returnIndex) 
{
  for (int i = 0; i < (int)currentBlock.size(); i++) 
  {
    if (currentBlock.at(i)->id == lookFor) 
    {
      returnIndex = i;
      return true;
    }
  }
  return false;
  
}


//---------------------------------------------------------------------------------------------
/**
 * @brief Defines a new entry in the current block table if not found.
 *
 * Defines a new TableEntry record using the input data, adds it to the current
 * block's vector of TableEntries, and returns true.
 * 
 * If the item ALREADY existed (i.e. we try to define X when X is already found) then
 * it returns false.
 * 
 * newsize and newvalue are used to define const/array sizes/values, which need
 * special handling. If the object is not a const or array, it will get a 0 for each.
 *
 */
bool BlockTable::define(int newid, Kind newkind, mType newtype, int newsize, int newvalue)
{
  //Check for matching existing TableEntry
  if (!search(newid)) 
  {
    TableEntry* entry = new TableEntry();
    
    entry->okind = newkind;
    entry->id = newid;
    entry->otype = newtype;
    entry->size = newsize;
    entry->value = newvalue;
    
    //add to the current block
    currentBlock.push_back(entry);
    return false; 
  } 
  else
    return true;
  
}
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
/**
 * @brief Defines a new entry in the current block table if not found.
 *
 * Defines a new TableEntry record using the input data, adds it to the current
 * block's vector of TableEntries, and returns true.
 * 
 * If the item ALREADY existed (i.e. we try to define X when X is already found) then
 * it returns false.
 *
 * This version of the function differs in that it is used for all non-const and
 * non-array types.
 *
 */
bool BlockTable::define(int newid, Kind newkind, mType newtype) 
{
  //we just pass in default values to our other function.
  //NOTE: A const CAN have a 0, so this may not be perfectly safe.
  return define(newid, newkind, newtype, 0, 0);
}
//---------------------------------------------------------------------------------------------

/**
 * @brief Changes the Size of an entry if said Array is found.
 *
 * Searches the current block for a given Array entry by
 * id. If found, use the index to change its size, and return true.
 * Otherwise (if not found) return false.
 *
 */
bool BlockTable::redefineSize(int id, int newsize) 
{
  int index = 0;
  //retrieve the index of an entry, if it exists
  if (search(id, index)) 
  {
    currentBlock.at(index)->size = newsize;
    return false;
  }
  //no such entry found to redefine!
  return true;
  
}

//---------------------------------------------------------------------------------------------
/**
 * @brief Searches this and all above blocks for a given TableEntry ID.
 * 
 * Searches for a given TableEntry by ID in all of the blocks above and
 * including the current one. This essentially searches all entries within
 * scope to see if one currently exists.
 *
 * If an entry is found, we pass back an "all ok" flag (error = false) and
 * directly return a copy of the given tableEntry itself.
 *
 * Otherwise, we return a blank table entry and activate our error flag
 * (if the item is not found).
 *
 */
TableEntry BlockTable::find(int lookfor, bool& error)
{
  //base iterators for our current block and vector of blocks
  vector< vector<TableEntry *> >::iterator it;
  vector<TableEntry *>::iterator it2;
  
  //checks the current blocks for the corresponding entry.
  for (it2 = currentBlock.begin(); it2 != currentBlock.end(); ++it2) 
  {
    if ((*it2)->id == lookfor) 
    {
      //the entry was found, so return it. No error.
      error = false;
      return *(*it2); 
    }
  }
  
  //check all of the previous blocks/scope for the given entry ID.
  for (int i = blocks.size()-1; i >= 0; i--) 
  {
    for (it2 = blocks.at(i).begin(); it2 != blocks.at(i).end(); ++it2) 
    {
      if ((*it2)->id == lookfor) 
      {
	error = false;
	return *(*it2); 
      }
    }	
  }
  
  //the item was not found, so we send an error and return an empty entry.
  error = true;
  TableEntry blank;
  return blank;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
/**
 * @brief Adds a new block to the BlockTable and pushes the old one onto the main vector.
 *
 * If our maximum scope-level has not been reached, then we put the current block onto the overall
 * "previous" vector (i.e. "higher level" of the scope).
 *
 * Really, we push a copy of it, and clear the current one, which allows us to save on memory and
 * reduce the risk of errors.
 *
 * As usual, a True is returned if a block could be successfully created, and a false otherwise.
 *
 */
bool BlockTable::newBlock() 
{
  
  if (currentBlockIndex < MAXBLOCK) 
  {
    blocks.push_back(currentBlock);
    currentBlockIndex++;
    currentBlock.clear();
    return true;
  }
  
  return false;
} 
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
/**
 * @brief Ends the current block/exits the top-most scope.
 *
 * We clear the current block information, which is synonymous logically
 * with ending the current given scope.
 *
 * All of the current tableEntrys go out of scope, and are thus cleared
 * along with the other block data.
 *
 * Additionally, we move the previous block data back into our temporary 
 * block, and remove it from the top of the Blocks vector.
 * This brings the BlockTable to the state it was in before it last raised
 * its scope.
 *
 */
bool BlockTable::endBlock() 
{
  //Sanity check to guarantee that we don't end a block without a scope!
  if (currentBlockIndex > 0) 
  {
    currentBlock.clear();
    
    currentBlockIndex--; 
    currentBlock = blocks.at(currentBlockIndex);
    blocks.erase(blocks.begin()+currentBlockIndex);
    return true;
  }
  return false;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
/**
 * @brief Prints out the current block with a "stock" title/heading
 *
 * Calls our main Block printing function with a default title of "Block"
 */
void BlockTable::printBlock(vector<TableEntry* > block) 
{
  printBlock(block, "Block");	
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
/**
 * @brief Helper Printer function, prints a block's information in an easy-to-read format.
 *
 * Prints all of the table-entry records in a given passed-in block.
 * A name parameter allows the caller to specificy specifics, such as the current
 * Parser function they are calling from.
 *
 * It utilizes our tableEntrytoString function to print the individual TableEntry records.
 *
 */
void BlockTable::printBlock(vector<TableEntry* > block, string name) 
{ 
  vector<TableEntry *>::iterator it;
  
  cout << "--------------------- " << name << " --------------------" << endl;
  cout << setw(4) << "ID" << setw(10) << "Kind" << setw(10) << "Type" << setw(10) << "Value" << setw(10) << "Size" << setw(10) << "Lexeme" << endl;
  for (it = block.begin(); it != block.end(); ++it) 
  {
    TableEntry* entry = *it;
    cout << tableEntrytoString(*entry);
  }
  cout << "------------------------------------------------------" << endl;
  
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
/**
 * @brief Returns the current block.
 *
 * Returns the current block, a vector of TableEntry pointers.
 */
vector<TableEntry* > BlockTable::returnCurrentBlock() 
{
  return currentBlock;	
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
/**
 * @brief Returns a given Kind of a TableEntry in string format.
 *
 * Converts a given Kind enum into a corresponding string.
 * Mostly for error checking.
 * If a given kind is not found, we report this.
 *
 */
string BlockTable::convertKind(int kind) 
{
  switch(kind) 
  {
    case CONSTANT: 
      return "Constant";
    case VARIABLE: 
      return "Variable";
    case ARRAY: 
      return "Array";
    case PROCEDURE: 
      return "Procedure";
    case DEFINED: 
      return "Defined";
    default:
      return "Undefined Kind (BlockTable::convertKind(int kind))";
  }
  return "Undefined Kind (BlockTable::convertKind(int kind))";
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
/**
 * @brief Returns a string representation of a Type enum.
 *
 * Returns a string representation of a given Type enum,
 * or returns an error message and the function name.
 *
 */
string BlockTable::convertType(int type) 
{
  switch(type) 
  {
    case INTEGER: 
      return "Integer";
    case BOOLEAN: 
      return "Boolean";
    case UNIVERSAL: 
      return "Universal";
    default: 
      return "Undefined Type (BlockTable::convertType(int type))";	
  }	
  return "Undefined Type (BlockTable::convertType(int type))";
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
/**
 * @brief Returns all of the Blocks
 *
 * Returns the Vector of the Blocks, a Vector of a Vector of TableEntry records.
 * Does not return the current block.
 *
 */
vector< vector<TableEntry* > > BlockTable::returnAllBlocks() 
{
  return blocks;	
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
/**
 * @brief Prints every Block in the BlockTable
 *
 * Uses our above helper functions to print out all of the Blocks in our
 * vector and then prints the current block.
 *
 */
void BlockTable::printAllBlocks() 
{
  vector< vector<TableEntry *> >::iterator it;
  
  for (it = blocks.begin(); it != blocks.end(); ++it) 
  {
    printBlock(*it);	
  }
  
  //Our current block is not on the stack.
  printBlock(currentBlock, "Current Block");
  
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
/**
 * @brief Converts a given TableEntry record to a String record type.
 * 
 * Creates a string with a specific formatting for outputting the given information of a 
 * specific Table Entry record.
 *
 */
string BlockTable::tableEntrytoString(TableEntry entry) 
{
  stringstream str;
  
  str << setw(4) << entry.id << setw(10) << convertKind(entry.okind) << setw(10) << convertType(entry.otype) << setw(10) 
      << entry.value << setw(10) << entry.size << setw(10) << table->getAttributeWhere(entry.id, "ID", "lexeme") << endl;
      
  return str.str();
}
//---------------------------------------------------------------------------------------------


