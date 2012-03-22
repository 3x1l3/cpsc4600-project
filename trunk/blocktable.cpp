#include "blocktable.h"
//---------------------------------------------------------------------------------------------
BlockTable::BlockTable(SymbolTable& symbolTable) {
	currentBlockIndex = 0;
	table = &symbolTable;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
bool BlockTable::search(int lookFor) {
	TableEntry tbl;
	return search(lookFor, tbl);
	
}
//---------------------------------------------------------------------------------------------


bool BlockTable::search(int lookFor, TableEntry& entry) {
	for (int i = 0; i < (int)currentBlock.size(); i++) {
		
		if (currentBlock.at(i)->id == lookFor) {
			entry = *currentBlock.at(i);
			
			return true;
		}
		
	}
	return false;
}

bool BlockTable::search(int lookFor, int& returnIndex) {
	for (int i = 0; i < (int)currentBlock.size(); i++) {
			if (currentBlock.at(i)->id == lookFor) {
				returnIndex = i;
				return true;
			}
		}
		return false;
	
}


//---------------------------------------------------------------------------------------------
bool BlockTable::define(int newid, Kind newkind, mType newtype, int newsize, int newvalue) {
	
	if (!search(newid)) {
	TableEntry* entry = new TableEntry();
	entry->okind = newkind;
	entry->id = newid;
	entry->otype = newtype;
	entry->size = newsize;
	entry->value = newvalue;
	currentBlock.push_back(entry);
	return false; 
	} else
	return true;
	
}
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
bool BlockTable::define(int newid, Kind newkind, mType newtype) {
	
	return define(newid, newkind, newtype, 0, 0);
		
}
//---------------------------------------------------------------------------------------------

bool BlockTable::redefineSize(int id, int newsize) {
	int index = 0;
	if (search(id, index)) {
		
		currentBlock.at(index)->size = newsize;
		
		return false;
	}
	return true;
	
}


//---------------------------------------------------------------------------------------------
TableEntry BlockTable::find(int lookfor, bool& error) {
  
	vector< vector<TableEntry *> >::iterator it;
	vector<TableEntry *>::iterator it2;
	
	for (it2 = currentBlock.begin(); it2 != currentBlock.end(); ++it2) {
			if ((*it2)->id == lookfor) {
				error = false;
				return *(*it2); 
			}
		}
			
	for (int i = blocks.size()-1; i >= 0; i--) {
		for (it2 = blocks.at(i).begin(); it2 != blocks.at(i).end(); ++it2) {
			if ((*it2)->id == lookfor) {
				error = false;
				return *(*it2); 
			}
		}	
	}
	
	
	
	error = true;
	TableEntry blank;
	return blank;
	
}	
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
bool BlockTable::newBlock() {
	
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
bool BlockTable::endBlock() {
	
	if (currentBlockIndex > 0) {
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
void BlockTable::printBlock(vector<TableEntry* > block, string name) {
	
	vector<TableEntry *>::iterator it;
	
	cout << "--------------------- " << name << " --------------------" << endl;
	cout << setw(4) << "ID" << setw(10) << "Kind" << setw(10) << "Type" << setw(10) << "Value" << setw(10) << "Size" << setw(10) << "Lexeme" << endl;
	for (it = block.begin(); it != block.end(); ++it) {
		TableEntry* entry = *it;
		cout << tableEntrytoString(*entry);
	}
	cout << "------------------------------------------------------" << endl;
	
}
//---------------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------------
void BlockTable::printBlock(vector<TableEntry* > block) {
	printBlock(block, "Block");	
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
vector<TableEntry* > BlockTable::returnCurrentBlock() {
	return currentBlock;	
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
string BlockTable::convertKind(int kind) {
	
	switch(kind) {
		case CONSTANT: return "Constant";
		case VARIABLE: return "Variable";
		case ARRAY: return "Array";
		case PROCEDURE: return "Procedure";
		case DEFINED: return "Defined";
		default:
			return "Undefined Kind (BlockTable::convertKind(int kind))";
	}
	
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
string BlockTable::convertType(int type) {
	switch(type) {
		case INTEGER: return "Integer";
		case BOOLEAN: return "Boolean";
		case UNIVERSAL: return "Universal";
		default: return "Undefined Type (BlockTable::convertType(int type))";	
	}	
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
vector< vector<TableEntry* > > BlockTable::returnAllBlocks() {
	return blocks;	
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
void BlockTable::printAllBlocks() {
	
	vector< vector<TableEntry *> >::iterator it;
	
	for (it = blocks.begin(); it != blocks.end(); ++it) {
		printBlock(*it);	
	}
	
	printBlock(currentBlock, "Current Block");
		
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
string BlockTable::tableEntrytoString(TableEntry entry) {
	stringstream str;
	str << setw(4) << entry.id << setw(10) << convertKind(entry.okind) << setw(10) << convertType(entry.otype) << setw(10) << entry.value << setw(10) << entry.size << setw(10) << table->getAttributeWhere(entry.id, "ID", "lexeme") << endl;
	return str.str();
}
//---------------------------------------------------------------------------------------------


