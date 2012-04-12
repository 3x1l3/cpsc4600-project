
#include "blocktable.h"


//---------------------------------------------------------------------------------------------

BlockTable::BlockTable(SymbolTable& symbolTable) 
{
  currentBlockIndex = 0;
  table = &symbolTable;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------

bool BlockTable::search(int lookFor)
{
  TableEntry tbl;
  return search(lookFor, tbl);  
}
//---------------------------------------------------------------------------------------------


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

//---------------------------------------------------------------------------------------------
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
    entry->level = currentBlockIndex;
    
    //sets the displacement variable
    //starting at 0 
    //0 is static link
    //1 is dynamic link
    //2 is return address
    //3 is first place for variables
    if(currentBlock.size() == 0)
    {
      entry->displacement = 3;
    }
    else
    {
      entry->displacement = ( 2 + (currentBlock.size() -1) );
    }
    
    
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

void BlockTable::printBlock(vector<TableEntry* > block) 
{
  printBlock(block, "Block");	
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------

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

vector<TableEntry* > BlockTable::returnCurrentBlock() 
{
  return currentBlock;	
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------

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

int BlockTable::currentLevel() {
	
	return currentBlockIndex;
	
}

