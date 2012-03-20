#include "blocktable.h"
//---------------------------------------------------------------------------------------------
BlockTable::BlockTable(SymbolTable& symbolTable) {
	currentBlockIndex = 0;
	table = &symbolTable;
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
bool BlockTable::search(int lookFor) {
	TableEntry* tbl;
	return search(lookFor, *tbl);
	
}
//---------------------------------------------------------------------------------------------


bool BlockTable::search(int lookFor, TableEntry& entry) {
	
	vector<TableEntry *>::iterator it;
	
	for(it = currentBlock.begin(); it != currentBlock.end(); ++it) {
		
		if ((*it)->id == lookFor) {
			entry = *(*it);
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
	return true; 
	} else
	return false;
	
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
bool BlockTable::define(int newid, Kind newkind, mType newtype) {
	
	return define(newid, newkind, newtype, 0, 0);
		
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
TableEntry BlockTable::find(int lookfor, bool& error) {
		vector< vector<TableEntry *> >::iterator it;
		vector<TableEntry *>::iterator it2;
		
	for (it = blocks.begin(); it != blocks.end(); ++it) {
		for (it2 = (*it).begin(); it2 != (*it).end(); ++it2) {
			if ((*it2)->id == lookfor) {
				error = true;
				return *(*it2); 
			}
		}	
	}
	
	for (it2 = currentBlock.begin(); it2 != currentBlock.end(); ++it2) {
			if ((*it2)->id == lookfor) {
				error = true;
				return *(*it2); 
			}
		}	
	
	error = false;
	TableEntry blank;
	return blank;
	
}	
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
bool BlockTable::newBlock() {
	
	if (currentBlockIndex < MAXBLOCK) {
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
		blocks.erase(blocks.begin()+currentBlockIndex	);
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
	cout << "----------------------------------------------" << endl;
	
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
			return "Kind Error";
	}
	
}
//---------------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------------
string BlockTable::convertType(int type) {
	switch(type) {
		case INTEGER: return "Integer";
		case BOOLEAN: return "Boolean";
		case UNIVERSAL: return "Universal";
		default: return "Type Error";	
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


