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
#include <stack>

/** 
 * A PL Program has two "parts" of any block, which are
 * a statement part and a definition part. We need to track these
 * for our semantic analysis.
 */
enum blocktype { STATEMENTPART, DEFINITIONPART };

/** Object definitions needed by the Parser. */
/** @see set.h, set.cpp */
#include "token.h"
#include "admin.h"
#include "set.h"	
#include "first.h"
#include "blocktable.h"

/** Namespaces needed gby the Parser. */
using std::string;
using std::cout;
using std::endl;
using std::stringstream;

/** Admin forward declaration to facilitate scanner control. */
class Admin;

class Parser {

private:
  
  /** Tokens retrieved using the scanner and used in parsing heuristics. */
  Token currentToken;
  Token lookAheadToken;
  Token prevToken;
  
  /** The Parser uses the Scanner to parse the file, through a pointer to the admin class. */
  Admin* admin;
  


  void Program(Set sts);
  void Block(int, int, Set sts);
  
  int DefinitionPart(Set sts);
  int Definition(int&, Set sts);
  void ConstantDefinition(Set sts);
  int VariableDefinition(int&, Set sts);
  int VariableDefinitionPart(Set sts, int&, mType type);
  
  
  mType TypeSymbol(Set sts); 
  vector<int> VariableList(Set sts, mType type, Kind kind);
  void ProcedureDefinition(Set sts);
  
  void StatementPart(Set sts);
  void Statement(Set sts);
  void EmptyStatement(Set sts);
  void ReadStatement(Set sts);
  
  /** We need to track the various types that make up an access list. */
  vector<mType> VariableAccessList(Set sts);
  
  void WriteStatement(Set sts);
  
  /** An Expression list can have a series of different types. */
  vector<mType> ExpressionList(Set sts);
  
  void AssignmentStatement(Set sts);
  void ProcedureStatement(Set sts);
  void IfStatement(Set sts);
  void DoStatement(Set sts);
  
  void GuardedCommandList(int&, int, Set sts);
  void GuardedCommand(int&, int, Set sts);
  
  mType Expression(Set sts);
  void PrimaryOperator(Set sts);
  mType PrimaryExpression(Set sts);
  void RelationalOperator(Set sts);
  mType SimpleExpression(Set sts);
  void AddingOperator(Set sts);
  mType Term(Set sts);
  void MultiplyingOperator(Set sts);
  mType Factor(Set sts);
  
  mType VariableAccess(Set sts);
  void IndexedSelector(Set sts);
  mType Constant(Set sts);
  mType Numeral(Set sts);
  mType BooleanSymbol(Set sts);
  
  mType ConstantName(Set sts);
  mType VariableName(Set sts);
  mType ProcedureName(Set sts);
  mType FactorName(Set sts);


  /**
   * @brief A function for printing debug parsing information to the screen.
   * This function takes the Function Name (a string), a given Set and a
   * Token as an input, and outputs them in a readable form.
   */
  void debug(string, Set, Token);
	
  /** The Debug function only processes if this Flag is set to True. */
  bool debugflag;
  
  /** A Symbol Table pointer, allowing direct access to Symbol Table information. */
  SymbolTable* table;
  
  /** A blocktable object used for proper Scope checking facilities. @see blocktable.h */
  BlockTable* blocktable;
  
  //A deprecated Array used for checking previous declarations.
  string prevMatch[4];
  //Deprecated, used in conjunction with prevmatch for previous scope attempt.
  int prevID;

  
  /** Used to track what area of the Block we are currently in, and thus what mType we should be returning. */
  stack<blocktype> blockTypeStack;
  
  /** Tracks the Scope type of parsing errors we encounter. */
  int numberOfScopeTypeErrors;

  int NewLabel();
  
public:
  
  /** Base constructor, requires the admin parent object that created it. */
  Parser(Admin&, SymbolTable&);
  
  /** Default Destructor. */
  ~Parser();
  
  /** Runs the Parser by starting the Parse production(s) tree with the EOL $ symbol. */
  void run();
  
  /** Uses the Admin object to run the scanner and grab the next token, parsing when valid. */
  Token nextToken();

  void match(string, Set);

  void syntaxError(Set);

  void syntaxCheck(Set);
  
  /** Keeps track of the Parsing Errors we encounter. */
  int errorCount;
  
  int labelCount;
};

#endif