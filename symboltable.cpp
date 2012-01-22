#include "symboltable.h"

/**
 * Default constructor for symboltable, just initialize index to 0.
 */
SymbolTable::SymbolTable()
{
    index = 0;
}

/**
 * Simply make an entry with the token name using the index variable and setting an empty string 
 * attribute to an empty string.
 * @param name
 */
int SymbolTable::makeEntry(std::string name)
{
    table[index][name][""] ="";
    int i = index;
    index++;
    
    return i;
}

int SymbolTable::makeEntry(std::string name, std::string attribute,
std::string value) {

    table[index][name][attribute] = value;
    int i = index;
    index++;
    
    return i;
    
}

/**
 * Add attribute knowing index, token name, attribute
 * @param index
 * @param name
 * @param attribute
 * @param value
 */
void SymbolTable::addAttribute(int index, std::string name, std::string attribute, std::string value) {
    table[index][name][attribute] = value;
}

/**
 * Simple get function to return the table
 * @return std::map<int, std::map<std::string, std::map<std::string, std::string> > >
 */
std::map<int, std::map<std::string, std::map<std::string, std::string> > > SymbolTable::get()
{
    return table;
}

/**
 * See if a given attribute exists based on index and name.
 * @param index
 * @param name
 * @param attribute
 * @return bool
 */
bool SymbolTable::attributeExists(int index, std::string name, std::string attribute) {
    std::string value = table[index][name][attribute];
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
std::string SymbolTable::findAttributeWhere(std::string tokenName, std::string attribute) {
    for (int i = 0; i < index; i++) {
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
int SymbolTable::findIndexWhere(std::string tokenName, std::string attribute) {
    for (int i=0; i < index; i++) {
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
bool SymbolTable::attributeValueExists(int index, std::string name, std::string attribute, std::string value) {
    if (attributeExists(index, name, attribute)) {
        std::string str = table[index][name][attribute];
        if ( str.compare(value) == 0)
            return true;
        return false;
    }
    return false;
}

std::string SymbolTable::getAttributeWhere(int index, std::string tokenName,
std::string attribute)
{
  if (attributeExists(index, tokenName, attribute)) {
    return table[index][tokenName][attribute];
  }
  
  return "";
}
