#include "parser.h"


Parser::Parser(Admin& adminObject)
{
  this->admin = &adminObject;
  lookAheadToken = nextToken();
}
/////////////////////////////////////////////////////////////////////////////
Parser::~Parser()
{
}
/////////////////////////////////////////////////////////////////////////////
void Parser::run()
{ 
  //sedn in money sign
  Program();
}
/////////////////////////////////////////////////////////////////////////////
Token Parser::nextToken()
{
  Token tok;
  do
  {
    tok = admin->nextToken();
    
  }while(tok.getType() == BADNUMERAL ||
         tok.getType() == BADSYMBOL ||
         tok.getType() == COMMENT ||
         tok.getType() == NEWLINE);
  
  return tok;
}
/////////////////////////////////////////////////////////////////////////////
void Parser::match(String matchMe, Set validNextCharacters)
{
  if (lookAheadToken.getLexeme() == matchMe) 
  {
    lookAheadToken = nextToken();
  }
  else
  {
    SyntaxError(validNextCharacters);
  }
  SyntaxCheck(validNextCharacters);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::SyntaxError( Set validNextCharcters)
{
  error("Syntax Error");
  
  while (! member(lookAheadToken.getLexeme(), validNextCharacters)) 
  {
    lookAheadToken = nextToken();
  }
}
/////////////////////////////////////////////////////////////////////////////
void Parser::SyntaxCheck(Set validNextCharcters)
{
  if (! member(lookAheadToken.getLexeme(), validNextCharacters))
  {
    SyntaxError(validNextCharacters);
  }
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void Program(Set sts)
{
  Set* temp = new Set(".");
  
  Block(sts.munion(temp)); 
  match(".", sts);
}
/////////////////////////////////////////////////////////////////////////////
void Block(Set sts)
{
  Set* temp = new Set("end");

  
  match         ("begin",sts.munion(First::DefinitionPart()).munion(First::StatementPart()).munion(*end));
  DefinitionPart(sts.munion(First::StatementPart()).munion(*end)); 
  StatementPart (sts.munion(*end)); 
  match         ("end", sts);
}
/////////////////////////////////////////////////////////////////////////////
void DefinitionPart(Set sts)
{
  Set* temp = new Set(";");
  set* first = First::Definition();
  
  //optional part
  //can be one or more, or nothing here
  //have to check if the lookahead is in the first of definition, and if not, 
  //then check if it is in the first of statement part from block
  while(First::Definition().isMember(lookAheadToken.getLexeme()))
  {
    Definition(sts.munion(temp)); 
    match(";", sts);
  }
  
  
  
}
/////////////////////////////////////////////////////////////////////////////
void Definition(Set sts)
{
  if(lookAheadToken.getLexeme() exists in First::ConstantDefintion())
  {
    ConstantDefinition();
  }
  if(lookAheadToken.getLexeme() exists in First::VariableDefinition())
  {
    VariableDefinition();
  }
  if(lookAheadToken.getLexeme() exists in First::ProcedureDefintion())
  {
    ProcedureDefintion();
  }
}
/////////////////////////////////////////////////////////////////////////////
void ConstantDefinition(Set sts)
{
  match("const"); ConstantName(); match("="); Constant();
}
/////////////////////////////////////////////////////////////////////////////
void VariableDefinition(Set sts)
{
  TypeSymbol(); VariableDefinitionPart();
}
/////////////////////////////////////////////////////////////////////////////
void VariableDefinitionPart(Set sts)
{
  VariableList();
  or
  match("array"); VariableList(); match("["); Constant(); match("]");
}
/////////////////////////////////////////////////////////////////////////////
void TypeSymbol(Set sts)
{
  match("integer");
  or
  match("Boolean");
}
/////////////////////////////////////////////////////////////////////////////
void VariableList(Set sts)
{
  VariableName();
  //optional part
  match(","); VariableName();
}
/////////////////////////////////////////////////////////////////////////////
void ProcedureDefintion(Set sts)
{
  match("proc"); ProcedureName(); Block();
}
/////////////////////////////////////////////////////////////////////////////
void StatementPart(Set sts)
{
  //optional
  Statement(); match(";");
}
/////////////////////////////////////////////////////////////////////////////
void Statement(Set sts)
{
  EmptyStatement();
  or
  ReadStatement();
  or
  WriteStatement();
  or
  AssignmentStatement();
  or
  ProcedureStatement();
  or
  IfStatement();
  or
  DoStatement();
  
}
/////////////////////////////////////////////////////////////////////////////
void EmptyStatement(Set sts)
{
    match("skip");
}
/////////////////////////////////////////////////////////////////////////////
void ReadStatement(Set sts)
{
  match("read"); VariableAccessList();
}
/////////////////////////////////////////////////////////////////////////////
void VariableAccessList(Set sts)
{
  VariableAccess();
  //optional
  match(","); VariableAccess();
}
/////////////////////////////////////////////////////////////////////////////
void WriteStatement(Set sts)
{
  match("write"); ExpressionList();
}
/////////////////////////////////////////////////////////////////////////////
void ExpressionList(Set sts)
{
  Expression();
  //optional
  match(","); Expression();
}
/////////////////////////////////////////////////////////////////////////////
void AssignmentStatement(Set sts)
{
  VariableAccessList(); match(":="); ExpressionList();
}
/////////////////////////////////////////////////////////////////////////////
void ProcedureStatement(Set sts)
{
  match("call");  ProcedureName();
}
/////////////////////////////////////////////////////////////////////////////
void IfStatement(Set sts)
{
  match("if"); GuardedCommandList(); match("fi");
}
/////////////////////////////////////////////////////////////////////////////
void DoStatement(Set sts)
{
  match("do"); GuardedCommandList(); match("od");
}
/////////////////////////////////////////////////////////////////////////////
void GuardedCommandList(Set sts)
{
  GuardedCommand();
  //optional
  match("[]"); GuardedCommand();
}
/////////////////////////////////////////////////////////////////////////////
void GuardedCommand(Set sts)
{
  Expression(); match("->"); StatementPart();
}
/////////////////////////////////////////////////////////////////////////////
void Expression(Set sts)
{
  PrimaryExpression();
  //optional
  PrimaryOperator(); PrimaryExpression();
}
/////////////////////////////////////////////////////////////////////////////
void PrimaryOperator(Set sts)
{
  match("&");
  or
  match("|");
}
/////////////////////////////////////////////////////////////////////////////
void PrimaryExpression(Set sts)
{
  SimpleExpression();
  //1 or zero of the follwing
  RelationalOperator(); SimpleExpression();
}
/////////////////////////////////////////////////////////////////////////////
void RelationalOperator(Set sts)
{
  match("<");
  or
  match("=");
  or
  match(">");
}
/////////////////////////////////////////////////////////////////////////////
void SimpleExpression(Set sts)
{
  //1 or zero of the following
  match("-");
  //required
  Term();
  //OPtional
  AddingOperator(); Term();
}
/////////////////////////////////////////////////////////////////////////////
void AddingOperator(Set sts)
{
  match("+");
  or 
  match("");
}
/////////////////////////////////////////////////////////////////////////////
void Term(Set sts)
{
  Factor();
  //optinal
  MultiplyingOperator(); Factor();
}
/////////////////////////////////////////////////////////////////////////////
void MultiplyingOperator(Set sts)
{
  match("*");
  or
  match("/");
  or
  match("\\");
}
/////////////////////////////////////////////////////////////////////////////
void Factor(Set sts)
{
  Constant();
  or
  VariableAccess();
  or
  match("("); Expression(); match(")");
  or
  match("~"); Factor();
}
/////////////////////////////////////////////////////////////////////////////
void VariableAccess(Set sts)
{
  VariableName();
  //one or zero of thefollowing
  IndexedSelector();
}
/////////////////////////////////////////////////////////////////////////////
void IndexedSelector(Set sts)
{
  match("["); Expression(); match("]");
}
/////////////////////////////////////////////////////////////////////////////
void Constant(Set sts)
{
  Numeral();
  or
  BooleanSymbol();
  or
  ConstantName();
}
/////////////////////////////////////////////////////////////////////////////
void Numeral(Set sts)
{
  //token must be a numeral
}
/////////////////////////////////////////////////////////////////////////////
void BooleanSymbol(Set sts)
{
  match("false");
  or
  match("true");
}
/////////////////////////////////////////////////////////////////////////////
void ConstantName(Set sts)
{
  //token must be a reserved word
}
/////////////////////////////////////////////////////////////////////////////
void VariableName(Set sts)
{
  //token must be a user defined word
}
/////////////////////////////////////////////////////////////////////////////