/**
 * @brief The function and object definition file for the Set class.
 *
 * @file set.cpp
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
 * Note that the Doxygen function comments have implicit briefs.
 *
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to March 1st, 2011
 **/

#include "set.h"

/** Base Constructor. */
Set::Set() 
{
	
	
}

/** String constructor, adds the given parameter to the set on construction.
 * @param str A lexeme to be added to the Set.
 */
Set::Set(string str) 
{
  set.push_back(str);	
}

/** 
 * @brief Vector constructor of the Set.
 * Creates the set using a Vector by straight assignment.
 * @param vec A string of lexemes/an unwrapped set.
 */
Set::Set(vector<string> vec) 
{
  set = vec;	
}

/** 
 * @brief Adds an input string to the Set object.
 * @param str A string lexeme to be added to the Set.
 */
void Set::add(string str)
{
  set.push_back(str);
}

/**
 * @brief Returns the core set as a Vector with no wrappings. 
 * @return this set in vector form.
 */
vector<string> Set::returnVec()
{
  return set;	
}

/** 
 * @brief Appends an input set to this one and returns the resultant set.
 *
 * A couple of temporary sets are used to perform the Vector modifications,
 * then the temporary set is returned.
 * This set is *not* modified. This is a function to return a proposed Union
 * set, not to actually union two sets.
 * This is done to maintain set integrity between nonterminal nodes.
 *
 * @param second_set A set to be appended to the caller set.
 */
Set Set::munion(Set second_set) 
{
  /** temporary sets to ensure original set integrity. */
  vector<string> sset = second_set.returnVec();
  vector<string> temp = set;
  
  /** Iteraten through the input set and add any unique values to the temp set. */
  for(unsigned int i=0; i < sset.size(); i++) 
  {
    if(!Set(temp).isMember(sset.at(i)))
      temp.push_back(sset.at(i));	
  }
  
  /** Return the set union to caller. */
  return Set(temp);
}

/** 
 * @brief Outputs the Set in basic set-builder-light notation. 
 * Constructs a string from the Vector, adding symbols such as {
 * and commas.
 * @return a string representation of the set.
 */
string Set::toString() 
{
  /** Loop through the set and add commas, etc. */
  string str = "{";
  for(unsigned int i=0; i < set.size(); i++) 
  {
    str += "'";
    str += set.at(i);
    
    str += "', ";
  }
  str += "}";
  
  return str;
  
}

/** 
 * @brief Remove function that Returns a copy of this set with element str removed. 
 * We use temporary sets and check for the existance of a string, then remove it
 * from the temp set and return it.
 *
 * If the string is not found, we return this original set.
 *
 * @param str String representing the lexeme to be "removed" from the return set.
 */
Set Set::remove(string str) 
{
  int index = indexAt(str);
  vector<string> temp;
  
  if (index != -1) 
  {
    temp = set;
    temp.erase(temp.begin()+index);
    return temp;
  }
  return set;	
}

/** 
 * @brief Finds the exact location of a given string within the Set and returns its index. 
 * We loop through the Set and return the index of the given string.
 * If it is not found, we return -1.
 * 
 * @param str String representing the parameter whose index we search for.
 */
int Set::indexAt(string str)
{
  for(unsigned int i=0; i < set.size(); i++) 
  {
    if (set.at(i) == str) 
    { 
      return i;	
    }
  }
  return -1;	
}

/**
 * @brief Checks to see if a given string is a member of the set.
 * We loop through the Set indices to determine if a given lexeme is present. 
 *
 * @param str String representing the lexeme we are searching for.
 */
bool Set::isMember(string str) 
{
  for(unsigned int i=0; i < set.size(); i++) 
  {
    if (set.at(i) == str) 
    {
      return true;	
    }
  }
  return false;
}