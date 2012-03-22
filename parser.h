/**
 * @brief The specification header for the Parser component object.
 *
 * @file parser.h
 * 
 * The Parser is responsible for properly syntactically verifying that the
 * input is correct, and (virtually) forming a syntax and grammatical tree
 * from the scanned input.
 *
 * It accomplishes this by linking back to the Scanner through the Admin,
 * using it to grab a given Token, then intelligently proceeding to the next
 * set of proper steps in the Grammatical structure of the PL Language. 
 *
 * Note that the Doxygen function documentation for these functions must be 
 * referred to from the CPP file as defining them again here would be not just 
 * redundant, but horrendously ugly.
 *
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to February 29th, 2011
 **/

#ifndef PARSER_H
#define PARSER_H

/** Standard STL output, string and container classes. */
#include <vector>
#include <string>
#include <iostream>
#include <sstream>

/** Object definitions needed by the Parser. */
/** @see set.h, set.cpp */
#include "token.h"
#include "admin.h"
#include "set.h"	
#include "first.h"
#include "blocktable.h"

/** Namespaces needed by the Parser. */
using std::string;
using std::cout;
using std::endl;
using std::stringstream;

/** Admin predeclaration to facilitate scanner control. */
class Admin;


class Parser {

private:
  
  /** Tokens retrieved using the scanner and used in parsing heuristics. */
  Token currentToken;
  Token lookAheadToken;
Token prevToken;
  
  /** The Parser uses the Scanner to parse the file, through a pointer to the admin class. */
  Admin* admin;
  
  
  /**
   * The following functions make up the core of our Parser, and are (in effect) essentially
   * just a visual and literal representation of the PL language's grammar rules; each nonterminal
   * production rule is equivalent to the calling of another one of these functions.
   * 
   */
  void Program(Set sts);
  void Block(Set sts);
  
  void DefinitionPart(Set sts);
  void Definition(Set sts);
  void ConstantDefinition(Set sts);
  void VariableDefinition(Set sts);
  void VariableDefinitionPart(Set sts, mType type);
  
  mType TypeSymbol(Set sts); 
  vector<int> VariableList(Set sts, mType type, Kind kind);
  void ProcedureDefintion(Set sts);
  
  void StatementPart(Set sts);
  void Statement(Set sts);
  void EmptyStatement(Set sts);
  void ReadStatement(Set sts);
  
  vector<mType> VariableAccessList(Set sts);
  void WriteStatement(Set sts);
  vector<mType> ExpressionList(Set sts);
  void AssignmentStatement(Set sts);
  void ProcedureStatement(Set sts);
  void IfStatement(Set sts);
  void DoStatement(Set sts);
  
  void GuardedCommandList(Set sts);
  void GuardedCommand(Set sts);
  
  mType Expression(Set sts);
  void PrimaryOperator(Set sts);
  void PrimaryExpression(Set sts);
  void RelationalOperator(Set sts);
  void SimpleExpression(Set sts);
  void AddingOperator(Set sts);
  void Term(Set sts);
  void MultiplyingOperator(Set sts);
  void Factor(Set sts);
  
  void VariableAccess(Set sts);
  void IndexedSelector(Set sts);
  void Constant(Set sts);
  void Numeral(Set sts);
  void BooleanSymbol(Set sts);
  
  void ConstantName(Set sts);
  void VariableName(Set sts);
  void ProcedureName(Set sts);
  void FactorName(Set sts);


  /**
   * @brief A function for printing debug parsing information to the screen.
   * This function takes the Function Name (a string), a given Set and a
   * Token as an input, and outputs them in a readable form.
   */
  void debug(string, Set, Token);
	
  /** The Debug function only processes if this Flag is set to True. */
  bool debugflag;
  
SymbolTable* table;
  BlockTable* blocktable;
  string prevMatch[4];
int prevID;

	void resetPrevMatches();

  
public:
  
  /** Base constructor, requires the admin parent object that created it. */
  Parser(Admin&, SymbolTable&);
  
  /** Default Destructor. */
  ~Parser();
  
  /** Runs the Parser by starting the Parse production(s) tree with the EOL $ symbol. */
  void run();
  
  /** Uses the Admin object to run the scanner and grab the next token, parsing when valid. */
  Token nextToken();
  
  /**
   * @brief Lexeme matching function.
   * 
   * Uses a set object consisting of the next possible valid lexemes to check of matched
   * character is valid, and outputs an error otherwise.
   * @param matchMe The string lexeme to be matched.
   * @param validNextCharacters The set of characters that represent a valid match.
   */
  void match(string, Set);
  
  /** 
   * @brief Outputs a syntax error message followed by the characters that would be correct. 
   * syntaxError will loop until it finds the next valid character after finding an error.
   * @param validNextCharacters The set of valid next characters as indicated by the grammar.
   */
  void syntaxError(Set);
  
  /** 
   * @brief A function that requests an error check upon finding a character that is invalid.
   * This can only happen after having followed a Valid input character.
   */
  void syntaxCheck(Set);
  
  int errorCount;
};

#endif