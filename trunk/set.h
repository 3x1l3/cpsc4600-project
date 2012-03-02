/**
 * @brief The specification header for the base Set class
 *
 * @file set.h
 * 
 * The Set class is essentially a wrapper for a Vector of Strings
 * that we use to get a more logical representation of a "set";
 *
 * That is to say, a mathematical set { "a", "b", "c", ...}
 * of lexemes or other string-represented values. 
 * Lexemetic values are stored for the Parser to use.
 *
 * Functions are provided to created mutual unions (munions)
 * of existing sets, as well as various other Set operations.
 * Though not all; only what the Parser needs.
 *
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to March 1st, 2011
 **/

#ifndef SET_H
#define SET_H

/** Basic STL inclusions to create a set. */
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>	//do we still need this? -A
#include <sstream>

/** Predefines for STL namespaces. */
using std::vector;
using std::string;
using std::cout;
using std::endl;

/**
 * @brief The set class manages logical sets of lexemetic values.
 
 * A SET is an object containing a vector of lexemic strings.
 *
 * An overarching Set stopset "sts" is "passed" between nonterminal
 * functions and their productions and is used to mediate the Matching;
 * Said stop set contains the valid values for continuing to parse, and
 * a lack of a specific value within it indicates a syntax error.
 *
 * The set object uses a Vector of strings to store lexemes.
 * The class also provides core functionality (altering, merging, accessing
 * and viewing the set) in a memory-efficient fashion.
 */
class Set {
  
  private:
    
    /** The core Set vector of strings itself, whose representation should never be seen. */
    vector<string> set;
	
  public:
    
    /** Base constructor. */
    Set();
    
    /** String constructor that creates a set using the passed Lexeme. */
    Set(string);
    
    /** A constructor that creates a Set out of a complete vector of strings, ie a semi-set. */
    Set(vector<string>);
    
    /** Appends the given String to the end of the Set vector. */
    void add(string);
    
    /** Searches for and removes a given String/lexeme from the Set if present, returning the resultant set. */
    Set remove(string);
    
    /** Returns the union of this set with another without actually modifying this set. */
    Set munion(Set);
    
    /** Returns the internal Vector of the set, ie the vector representation of the Set object. */
    vector<string> returnVec();
    
    /** Returns a logical String representation of the Set for OUTPUT/user preview/debug purposes only. */
    string toString();
    
    /** Checks to see if a given lexeme is in the Set. */
    bool isMember(string);
    
    /** Returns the exact location, within the set, of a given lexeme/input string. */
    int indexAt(string);
	
};

#endif