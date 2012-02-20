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
void Program(Set sts)
{
  Block(sts.munion(First::Block()); match(".");
}
/////////////////////////////////////////////////////////////////////////////
void Block(Set sts)
{
  Set* end = new Set();
  end->add("end");
  
  Set* one = new Set();
  Set* two = new Set();
  Set* three = new Set();
  Set* four = new Set();
  
  match("begin", sts.munion(First::DefinitionPart().munion(First::StatementPart()).munion(*end)); 
  DefinitionPart(); 
  StatementPart(); 
  match("end");
}
/////////////////////////////////////////////////////////////////////////////
void DefinitionPart()
{
  //optional part
  //can be one or more, or nothing here
  //have to check if the lookahead is in the first of definition, and if not, then check if it is in the first of statement part from block
  Definition(); match(";");
}
/////////////////////////////////////////////////////////////////////////////
void Definition()
{
  ConstantDefinition();
  or
  VariableDefinition();
  or
  ProcedureDefintion();
}
/////////////////////////////////////////////////////////////////////////////
void ConstantDefinition()
{
  match("const"); ConstantName(); match("="); Constant();
}
/////////////////////////////////////////////////////////////////////////////
void VariableDefinition()
{
  TypeSymbol(); VariableDefinitionPart();
}
/////////////////////////////////////////////////////////////////////////////
void VariableDefinitionPart()
{
  VariableList();
  or
  match("array"); VariableList(); match("["); Constant(); match("]");
}
/////////////////////////////////////////////////////////////////////////////
void TypeSymbol()
{
  match("integer");
  or
  match("Boolean");
}
/////////////////////////////////////////////////////////////////////////////
void VariableList()
{
  VariableName();
  //optional part
  match(","); VariableName();
}
/////////////////////////////////////////////////////////////////////////////
void ProcedureDefintion()
{
  match("proc"); ProcedureName(); Block();
}
/////////////////////////////////////////////////////////////////////////////
void StatementPart()
{
  //optional
  Statement(); match(";");
}
/////////////////////////////////////////////////////////////////////////////
void Statement()
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
void EmptyStatement()
{
    match("skip");
}
/////////////////////////////////////////////////////////////////////////////
void ReadStatement()
{
  match("read"); VariableAccessList();
}
/////////////////////////////////////////////////////////////////////////////
void VariableAccessList()
{
  VariableAccess();
  //optional
  match(","); VariableAccess();
}
/////////////////////////////////////////////////////////////////////////////
void WriteStatement()
{
  match("write"); ExpressionList();
}
/////////////////////////////////////////////////////////////////////////////
void ExpressionList()
{
  Expression();
  //optional
  match(","); Expression();
}
/////////////////////////////////////////////////////////////////////////////
void AssignmentStatement()
{
  VariableAccessList(); match(":="); ExpressionList();
}
/////////////////////////////////////////////////////////////////////////////
void ProcedureStatement()
{
  match("call");  ProcedureName();
}
/////////////////////////////////////////////////////////////////////////////
void IfStatement()
{
  match("if"); GuardedCommandList(); match("fi");
}
/////////////////////////////////////////////////////////////////////////////
void DoStatement()
{
  match("do"); GuardedCommandList(); match("od");
}
/////////////////////////////////////////////////////////////////////////////
void GuardedCommandList()
{
  GuardedCommand();
  //optional
  match("[]"); GuardedCommand();
}
/////////////////////////////////////////////////////////////////////////////
void GuardedCommand()
{
  Expression(); match("->"); StatementPart();
}
/////////////////////////////////////////////////////////////////////////////
void Expression()
{
  PrimaryExpression();
  //optional
  PrimaryOperator(); PrimaryExpression();
}
/////////////////////////////////////////////////////////////////////////////
void PrimaryOperator()
{
  match("&");
  or
  match("|");
}
/////////////////////////////////////////////////////////////////////////////
void PrimaryExpression()
{
  SimpleExpression();
  //1 or zero of the follwing
  RelationalOperator(); SimpleExpression();
}
/////////////////////////////////////////////////////////////////////////////
void RelationalOperator()
{
  match("<");
  or
  match("=");
  or
  match(">");
}
/////////////////////////////////////////////////////////////////////////////
void SimpleExpression()
{
  //1 or zero of the following
  match("-");
  //required
  Term();
  //OPtional
  AddingOperator(); Term();
}
/////////////////////////////////////////////////////////////////////////////
void AddingOperator()
{
  match("+");
  or 
  match("");
}
/////////////////////////////////////////////////////////////////////////////
void Term()
{
  Factor();
  //optinal
  MultiplyingOperator(); Factor();
}
/////////////////////////////////////////////////////////////////////////////
void MultiplyingOperator()
{
  match("*");
  or
  match("/");
  or
  match("\\");
}
/////////////////////////////////////////////////////////////////////////////
void Factor()
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
void VariableAccess()
{
  VariableName();
  //one or zero of thefollowing
  IndexedSelector();
}
/////////////////////////////////////////////////////////////////////////////
void IndexedSelector()
{
  match("["); Expression(); match("]");
}
/////////////////////////////////////////////////////////////////////////////
void Constant()
{
  Numeral();
  or
  BooleanSymbol();
  or
  ConstantName();
}
/////////////////////////////////////////////////////////////////////////////
void Numeral()
{
  //token must be a numeral
}
/////////////////////////////////////////////////////////////////////////////
void BooleanSymbol()
{
  match("false");
  or
  match("true");
}
/////////////////////////////////////////////////////////////////////////////
void ConstantName()
{
  //token must be a reserved word
}
/////////////////////////////////////////////////////////////////////////////
void VariableName()
{
  //token must be a user defined word
}
/////////////////////////////////////////////////////////////////////////////