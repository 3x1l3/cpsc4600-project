#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <map>
#include <iostream>

using std::string;
using std::map;

class SymbolTable {

private:
    /**
     * Lets define the symbol table as a map, key being an integer index. The second value
     * is another map, key being the name of the token and the value being yet another map
     * key being any attribute such as "lexeme" or "type" and that map's value is the value
     * for that attribute.
     *
     * So essentially we can get an item's lexeme by using the following:
     * table[34]["ID"]["lexeme"]
     *
     * This will allow us to store any number of attributes that we need to store in the symbol table.
     *
     * Might be considered to be implimented using vectors instead.
     */
     map<int, map<string, map<string, string> > > table;

    /**
     * Lets say we auto increment an index so for each make entry we'll just used an auto assigned
     * integer.
     */
    int index;

public:
    SymbolTable();
    int makeEntry(string);
    int makeEntry(string, string, string);
    void addAttribute(int, string, string, string);
    map<int, map<string, map<string, string> > > get();
    bool attributeExists(int, string, string);
    string findAttributeWhere(string, string);
    int findIndexWhere(string, string);
    bool attributeValueExists(int, string, string, string);
    string getAttributeWhere(int, string, string);
    int findLexeme(string);
    int numberOfReservedWords;
    int getNumOfReservedWords();
};

#endif