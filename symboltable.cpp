/**
 * @brief The formal definition for our SymbolTable (symboltable).
 *
 * @file symboltable.cpp
 * 
 *	We create a symbol table using a cascading order of Maps.
 * This table structure object provides a variety of ways to access
 * and alter entries in the table, and to make new ones and verify
 * the integrity of old ones.
 *
 * @see SymbolTable{};
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to 31st, 2011
 **/

#include "symboltable.h"

/**
 * @brief Default constructor for symboltable, initializes index to 0 and sets reserved words.
 * 
 * We create a bunch of entries in our symbol table to reflect Reserved Words of the PL Language.
 * The language specifies 17 particular reserved words, which we also store as an integer count.
 * We set our index before making these entries.
 */
SymbolTable::SymbolTable()
{
    index = 0;
    numberOfReservedWords = 17;
    makeEntry("ID","lexeme", "begin");
    makeEntry("ID","lexeme", "end");
    makeEntry("ID","lexeme", "const");
    makeEntry("ID","lexeme", "array");
    makeEntry("ID","lexeme", "integer");
    makeEntry("ID","lexeme", "boolean");
    makeEntry("ID","lexeme", "proc");
    makeEntry("ID","lexeme", "skip");
    makeEntry("ID","lexeme", "read");
    makeEntry("ID","lexeme", "write");
    makeEntry("ID","lexeme", "call");
    makeEntry("ID","lexeme", "if");
    makeEntry("ID","lexeme", "do");
    makeEntry("ID","lexeme", "fi");
    makeEntry("ID","lexeme", "od");
    makeEntry("ID","lexeme", "false");
    makeEntry("ID","lexeme", "true");
}

/** Returns the reserved word count. */
int SymbolTable::getNumOfReservedWords()
{
    return numberOfReservedWords;
}

/**
 * @brief An entry to the table is made with no information other than an input Name variable.
 * The index variable is used for the index location of the new entry, and the rest of the entry is empty.
 * Our index is increased and the former index is returned.
 * @param name
 */
int SymbolTable::makeEntry(string name)
{
    table[index][name][""] ="";
    int i = index;
    index++;
    
    return i;
}

/** 
 * A new table entry is made with a predetermined name, lexeme/attribute and value of said attribute. 
 * @param name
 * @param attribute
 * @param value
 */
int SymbolTable::makeEntry(string name, string attribute, string value) 
{
    table[index][name][attribute] = value;
    int i = index;
    index++;
    
    return i;
}

/**
 * Adds/Alters an attribute at a specific index, specifying token name, attribute and value.
 * @param index
 * @param name
 * @param attribute
 * @param value
 */
void SymbolTable::addAttribute(int index, string name, string attribute, string value) 
{
    table[index][name][attribute] = value;
}

/**
 * This function returns a copy of the Table.
 * @return std::map<int, std::map<std::string, std::map<std::string, std::string> > > Symbol Table.
 */
map<int, map<string, map<string, string> > > SymbolTable::get()
{
    return table;
}

/**
 * Checks if a given attribute location exists based on an index and name.
 *
 * It is important to note that this is not checking to see if that attribute
 * has an actual value assigned to it, but merely that an entry that can house
 * a value exists.
 * @param index
 * @param name
 * @param attribute
 * @return bool
 */
bool SymbolTable::attributeExists(int index, string name, string attribute) 
{
    string value = table[index][name][attribute];
    return !value.empty();
}

/**
 * Return an attribute value given the tokenName and attribute. Not sure how useful this will
 * be later. IF there are multiple tokens with the same attribute names then this will return
 * the first find. Which makes me believe this wont be very useful.
 * @param tokenName
 * @param attribute
 * @return attribute value on success, empty string on fail.
 */
string SymbolTable::findAttributeWhere(string tokenName, string attribute) 
{
    for (int i = 0; i < index; i++) 
    {
      if ( attributeExists(i, tokenName, attribute) )
	return table[i][tokenName][attribute];
    }
    return "";
}

/**
 * Find an index using a token name and attribute name. Not sure how useful this will be
 * @param tokenName
 * @param attribute
 * @return index of found attribute, -1 on fail
 */
int SymbolTable::findIndexWhere(string tokenName, string attribute) 
{
    for (int i=0; i < index; i++) 
    {
      if (attributeExists(i, tokenName, attribute))
	return i;
    }
    return -1;
}

/**
 * Find if an attribute value exists given the information provided.
 * @param index
 * @param names
 * @param attribute
 * @param value
 * @return bool
 */
bool SymbolTable::attributeValueExists(int index, string name, string attribute, string value) 
{
    if (attributeExists(index, name, attribute)) 
    {
      string str = table[index][name][attribute];
      if ( str.compare(value) == 0)
	return true;
        return false;
    }
    return false;
}

/** 
 * Returns a value at a given location if it exists.
 * Otherwise, we return the empty string.
 * @param index
 * @param tokenName
 * @param attribute
 */
string SymbolTable::getAttributeWhere(int index, string tokenName, string attribute)
{
    if (attributeExists(index, tokenName, attribute)) 
    {
      return table[index][tokenName][attribute];
    }
  
    return "";
}

/**
 * Finds a table entry with the given lexeme. 
 * This assumes the entry is an ID and attribute is a lexeme. 
 * It will return the index of the found entry, or
 * a -1 if an entry is not found.
 * @return int Integer representing the index of the found lexeme.
 */
int SymbolTable::findLexeme(string lexeme)
{
    for(int i=0; i < index; i++) 
    {
      if (table[i]["ID"]["lexeme"] == lexeme)
	return i;
    }
    return -1;
}