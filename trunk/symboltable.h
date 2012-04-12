/**
 * @brief The Interface/Specification for our SymbolTable (symboltable).
 *
 * @file symboltable.h
 * 
 * The Symbol Table (symboltable) object is a core object used by the Scanner and 
 * Parser and the majority of the compiler altogether.
 *
 * We impliment the symbol table as a Map of a Map of a Map.
 * The reason behind this is that it allows any given component of the table
 * to have multiple values if needed. Additionally, the nested Map structure
 * allows for fast searching and direct access as well.
 *
 * Traditionally, a Symbol Table will be constructed for each "block"
 * of nesting we enter.
 *
 * It should be noted that "lexeme" is used frequently but in most cases
 * the given function / table entry may not be a traditional lexeme at all.
 * We provide functions to find a symbol table entry with any 2 given entry values.
 *
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to 31st, 2011
 **/

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

/** Header Libraries used by the symboltable. */
#include <string>
#include <map>
#include <iostream>

/** Useful predefines for the symboltable. */
using std::string;
using std::map;

/**
 * @brief symboltable object that stores lexemes, IDentifiers and Locations as keys.
 *
 * The symbol table stores its values in a Map of a Map of a Map.
 * The values themselves are the specific keys of each kind.
 * 
 * It provides multiple ways to access and change inner Symbol Table data, 
 * and stores said data in a fast access form.
 * 
 * The structure is thus able to scale vertically and horizontally by introducing
 * more linked maps, or storing more key values and attributes.
 *
 * For reference, String Compare returns a 0 if the comparison result is fully "true".
 */
class SymbolTable {

private:
    /**
     * @brief Our symbol table is a Map with a Map with a Map, called "table".
     * 
     * We define the symbol table using a map, the key being an integer index. 
     * The second value is another map, its key being the name of the token and the
     * value being yet another map with its key being any attribute such as "lexeme" 
     * or "type" and that map's value is the value for that attribute.
     *
     * So essentially we can get an item's lexeme by using the following:
     * table[34]["ID"]["lexeme"]
     *
     * This will allow us to store any number of attributes that we need to store in the symbol table.
     * So it would be possible to add multiple lexemes for a given word.
     * Essentially, this is akin to storing a linked list of attribute sets
     * in a hashmap using linear probing.
     *
     * This could also be accomplished with a Vector, but Vectors scale at a worse rate than a Map.
     */
    map< int, map< string, map< string, string > > > table;
	 

    /**
     * @brief An integer to index our symbol table easier.
     *
     * We use an automatically incrementing index for each entry we make, so we'll just store an
     * auto-assigned integer within this symbol table.
     */
    int index;

public:

    /** Basic constructor. */
    SymbolTable();
    ~SymbolTable() { } 

    /** Adds another entry to table and increases the index then returning the previous index value. */
    int makeEntry(string);
    int makeEntry(string, string, string);
    
    /** Assigns a new value to the lexeme at the specified location. */
    void addAttribute(int, string, string, string);
    
    /** Returns a copy of the current symbol table. */
    map<int, map<string, map<string, string> > > get();
    
    /** Checks if a given entry/lexeme actually exists or not. This is separate from the lexeme's value. */
    bool attributeExists(int, string, string);

    /** 
     * @brief Verify that the attribute value at the given entry matches the one the caller is examining.
     * We check if the given lexeme exists, then check if the Value of it is equivalent to 
     * the one sent by the caller (the final String "value").
     * Could be used for Macro shorthands, will likely be used by the Parser.
     */
    bool attributeValueExists(int, string, string, string);

    /** 
     * @brief Searches the table for an entry with a matching lexeme and token name. 
     * Returns nothing if the attribute is not found.
     * Otherwise it returns the attribute string at that specific lexemetic location.
     */
    string findAttributeWhere(string, string);

    /** Returns the value at the given Attribute location if it exists. */
    string getAttributeWhere(int, string, string);

    /** We find the Index of a token entry using its name and attribute. */
    int findIndexWhere(string, string);

    /** 
     * Returns an integer representing the location of a lexeme, so long as it is an ID and actual lexeme. 
     * Otherwise, it returns -1.
     */
    int findLexeme(string);

    /**  We store and return the number of possible reserved words. This is static and specified by the language. */
    int numberOfReservedWords;
    int getNumOfReservedWords();
    
    /** Checks if a given index is within or at the end of the symbol table. */
    bool indexExists(int);
};

#endif