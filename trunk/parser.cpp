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
    Set* temp = new Set("$");
  //sedn in money sign
  this->Program(*temp);
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
void Parser::match(string matchMe, Set validNextCharacters)
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
void Parser::syntaxError( Set validNextCharcters)
{
  error("Syntax Error");
  
  while (! member(lookAheadToken.getLexeme(), validNextCharacters)) 
  {
    lookAheadToken = nextToken();
  }
}
/////////////////////////////////////////////////////////////////////////////
void Parser::syntaxCheck(Set validNextCharcters)
{
  if (! member(lookAheadToken.getLexeme(), validNextCharacters))
  {
    SyntaxError(validNextCharacters);
  }
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void Parser::Program(Set sts)
{
  Set* temp = new Set(".");
  
  Block(sts.munion(temp)); 
  match(".", sts);
  
  syntaxCheck(sts);
  
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Block(Set sts)
{
  Set* temp = new Set("end");

  
  match         ("begin",sts.munion(First::DefinitionPart()).munion(First::StatementPart()).munion(*end));
  DefinitionPart(sts.munion(First::StatementPart()).munion(*end)); 
  StatementPart (sts.munion(*end)); 
  match         ("end", sts);
  
  syntaxCheck(sts);
  
  
}
/////////////////////////////////////////////////////////////////////////////
void Parser::DefinitionPart(Set sts)
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
  
  syntaxCheck(sts);
  
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Definition(Set sts)
{
  if(First::ConstantDefinition().isMember(lookAheadToken.getLexeme()))
  {
    ConstantDefinition();
  }
  if(First::VariableDefinition().isMember(lookAheadToken.getLexeme()))
  {
    VariableDefinition();
  }
  if(First::ProcedureDefinition().isMember(lookAheadToken.getLexeme()))
  {
    ProcedureDefintion();
  }
  
  syntaxCheck(sts);
  
  
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ConstantDefinition(Set sts)
{
  match("const"); ConstantName(); match("="); Constant();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableDefinition(Set sts)
{
  TypeSymbol(); VariableDefinitionPart();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableDefinitionPart(Set sts)
{
  VariableList();
  or
  match("array"); VariableList(); match("["); Constant(); match("]");
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::TypeSymbol(Set sts)
{
  match("integer");
  or
  match("Boolean");
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableList(Set sts)
{
  VariableName();
  //optional part
  match(","); VariableName();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ProcedureDefintion(Set sts)
{
  match("proc"); ProcedureName(); Block();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::StatementPart(Set sts)
{
  //optional
  Statement(); match(";");
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Statement(Set sts)
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
  
  syntaxCheck(sts);
  
}
/////////////////////////////////////////////////////////////////////////////
void Parser::EmptyStatement(Set sts)
{
    match("skip");
    
    syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ReadStatement(Set sts)
{
  match("read"); VariableAccessList();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableAccessList(Set sts)
{
  VariableAccess();
  //optional
  match(","); VariableAccess();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::WriteStatement(Set sts)
{
  match("write"); ExpressionList();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ExpressionList(Set sts)
{
  Expression();
  //optional
  match(","); Expression();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::AssignmentStatement(Set sts)
{
  VariableAccessList(); match(":="); ExpressionList();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ProcedureStatement(Set sts)
{
  match("call");  ProcedureName();
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::IfStatement(Set sts)
{
  match("if"); GuardedCommandList(); match("fi");
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::DoStatement(Set sts)
{
  match("do"); GuardedCommandList(); match("od");
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::GuardedCommandList(Set sts)
{
  GuardedCommand();
  //optional
  match("[]"); GuardedCommand();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::GuardedCommand(Set sts)
{
  Expression(); match("->"); StatementPart();
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Expression(Set sts)
{
  PrimaryExpression();
  //optional
  PrimaryOperator(); PrimaryExpression();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::PrimaryOperator(Set sts)
{
  match("&");
  or
  match("|");
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::PrimaryExpression(Set sts)
{
  SimpleExpression();
  //1 or zero of the follwing
  RelationalOperator(); SimpleExpression();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::RelationalOperator(Set sts)
{
  match("<");
  or
  match("=");
  or
  match(">");
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::SimpleExpression(Set sts)
{
  //1 or zero of the following
  match("-");
  //required
  Term();
  //OPtional
  AddingOperator(); Term();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::AddingOperator(Set sts)
{
  match("+");
  or 
  match("");
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Term(Set sts)
{
  Factor();
  //optinal
  MultiplyingOperator(); Factor();
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::MultiplyingOperator(Set sts)
{
  match("*");
  or
  match("/");
  or
  match("\\");
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Factor(Set sts)
{
  Constant();
  or
  VariableAccess();
  or
  match("("); Expression(); match(")");
  or
  match("~"); Factor();
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableAccess(Set sts)
{
  VariableName();
  //one or zero of thefollowing
  IndexedSelector();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::IndexedSelector(Set sts)
{
  match("["); Expression(); match("]");
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Constant(Set sts)
{
  Numeral();
  or
  BooleanSymbol();
  or
  ConstantName();
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Numeral(Set sts)
{
  //token must be a numeral
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::BooleanSymbol(Set sts)
{
  match("false");
  or
  match("true");
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ConstantName(Set sts)
{
  //token must be a reserved word
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableName(Set sts)
{
  //token must be a user defined word
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////