

#ifndef PARSER_H
#define PARSER_H


#include <vector>
#include <string>
#include <iostream>
#include <sstream>


#include "token.h"
#include "admin.h"

using std::string;
using std::cout;
using std::endl;
using std::stringstream;

class Admin;

class Parser {

private:
  
  Token currentToken;
  Token lookAheadToken;
  Admin * admin;
  

  void Program();
  void Block();
  void DefinitionPart();
  void Definition();
  void ConstantDefinition();
  void VariableDefinition();
  void VariableDefinitionPart();
  void TypeSymbol();
  void VariableList();
  void ProcedureDefintion();
  void StatementPart();
  void Statement();
  void EmptyStatement();
  void ReadStatement();
  void VariableAccessList();
  void WriteStatement();
  void ExpressionList();
  void AssignmentStatement();
  void ProcedureStatement();
  void IfStatement();
  void DoStatement();
  void GuardedCommandList();
  void GuardedCommand();
  void Expression();
  void PrimaryOperator();
  void PrimaryExpression();
  void RelationalOperator();
  void SimpleExpression();
  void AddingOperator();
  void Term();
  void MultiplyingOperator();
  void Factor();
  void VariableAccess();
  void IndexedSelector();
  void Constant();
  void Numeral();
  void BooleanSymbol();
  void ConstantName();
  void VariableName();


  
public:
  
  Parser(Admin& );
  ~Parser();
  void run();
  Token nextToken();
};

#endif