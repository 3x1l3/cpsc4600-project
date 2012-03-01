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
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to February 29th, 2011
 **/

#ifndef PARSER_H
#define PARSER_H


#include <vector>
#include <string>
#include <iostream>
#include <sstream>


#include "token.h"
#include "admin.h"
#include "set.h"
#include "first.h"

using std::string;
using std::cout;
using std::endl;
using std::stringstream;

class Admin;


class Parser {

private:
  
  Token currentToken;
  Token lookAheadToken;
  Admin* admin;

  

  void Program(Set sts);
  void Block(Set sts);
  void DefinitionPart(Set sts);
  void Definition(Set sts);
  void ConstantDefinition(Set sts);
  void VariableDefinition(Set sts);
  void VariableDefinitionPart(Set sts);
  void TypeSymbol(Set sts);
  void VariableList(Set sts);
  void ProcedureDefintion(Set sts);
  void StatementPart(Set sts);
  void Statement(Set sts);
  void EmptyStatement(Set sts);
  void ReadStatement(Set sts);
  void VariableAccessList(Set sts);
  void WriteStatement(Set sts);
  void ExpressionList(Set sts);
  void AssignmentStatement(Set sts);
  void ProcedureStatement(Set sts);
  void IfStatement(Set sts);
  void DoStatement(Set sts);
  void GuardedCommandList(Set sts);
  void GuardedCommand(Set sts);
  void Expression(Set sts);
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


  //Helper function, debug
  void debug(string, Set, Token);
	

  bool debugflag;

  
public:
  
  Parser(Admin& );
  ~Parser();
  void run();
  Token nextToken();
  void match(string, Set);
  void syntaxError(Set);
  void syntaxCheck(Set);
};

#endif