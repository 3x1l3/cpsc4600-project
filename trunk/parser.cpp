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
    syntaxError(validNextCharacters);
  }
  syntaxCheck(validNextCharacters);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::syntaxError( Set validNextCharacters)
{
  cout<<"Syntax Error"<<endl;
  
  while (! validNextCharacters.isMember(lookAheadToken.getLexeme())) 
  {
    lookAheadToken = nextToken();
  }
}
/////////////////////////////////////////////////////////////////////////////
void Parser::syntaxCheck(Set validNextCharacters)
{
  if (! validNextCharacters.isMember(lookAheadToken.getLexeme()))
  {
    syntaxError(validNextCharacters);
  }
}



/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
void Parser::Program(Set sts)
{
  Set* temp = new Set(".");
  
  Block(sts.munion(*temp)); 
  match(".", sts);
  
  syntaxCheck(sts);
  
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Block(Set sts)
{
  Set* temp = new Set("end");

  
  match         ("begin",sts.munion(First::DefinitionPart()).munion(First::StatementPart()).munion(*temp));
  DefinitionPart(sts.munion(First::StatementPart()).munion(*temp)); 
  StatementPart (sts.munion(*temp)); 
  match         ("end", sts);
  
  syntaxCheck(sts);
  
  
}
/////////////////////////////////////////////////////////////////////////////
void Parser::DefinitionPart(Set sts)
{
  Set* temp = new Set(";");
  Set first = First::Definition();
  
  //optional part
  //can be one or more, or nothing here
  //have to check if the lookahead is in the first of definition, and if not, 
  //then check if it is in the first of statement part from block
  while(first.isMember(lookAheadToken.getLexeme()))
  {
    Definition(sts.munion(*temp)); 
    match(";", sts);
  }
  
  syntaxCheck(sts);
  
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Definition(Set sts)
{
  if(First::ConstantDefinition().isMember(lookAheadToken.getLexeme()))
  {
    ConstantDefinition(sts);
  }
  else if(First::VariableDefinition().isMember(lookAheadToken.getLexeme()))
  {
    VariableDefinition(sts);
  }
  else if(First::ProcedureDefinition().isMember(lookAheadToken.getLexeme()))
  {
    ProcedureDefintion(sts);
  }
  
  syntaxCheck(sts);
  
  
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ConstantDefinition(Set sts)
{
  Set* temp = new Set("=");
  
  
  match("const", sts.munion(First::ConstantName()).munion(*temp).munion(First::Constant())); 
  ConstantName(sts.munion(*temp).munion(First::Constant())); 
  match("=", sts.munion(First::Constant())); 
  Constant(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableDefinition(Set sts)
{
  TypeSymbol(sts); 
  VariableDefinitionPart(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableDefinitionPart(Set sts)
{
  VariableList(sts);
  //or
  match("array",sts); VariableList(sts); match("[",sts); Constant(sts); match("]",sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::TypeSymbol(Set sts)
{
  match("integer", sts);
  //or
  match("Boolean", sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableList(Set sts)
{
  VariableName(sts);
  //optional part
  match(",",sts); VariableName(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ProcedureDefintion(Set sts)
{
  match("proc",sts); ProcedureName(sts); Block(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::StatementPart(Set sts)
{
  //optional
  Statement(sts); match(";",sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Statement(Set sts)
{
  EmptyStatement(sts);
  //or
  ReadStatement(sts);
  //or
  WriteStatement(sts);
  //or
  AssignmentStatement(sts);
  //or
  ProcedureStatement(sts);
  //or
  IfStatement(sts);
  //or
  DoStatement(sts);
  
  syntaxCheck(sts);
  
}
/////////////////////////////////////////////////////////////////////////////
void Parser::EmptyStatement(Set sts)
{
    match("skip",sts);
    
    syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ReadStatement(Set sts)
{
  match("read",sts); VariableAccessList(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableAccessList(Set sts)
{
  VariableAccess(sts);
  //optional
  match(",",sts); VariableAccess(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::WriteStatement(Set sts)
{
  match("write",sts); ExpressionList(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ExpressionList(Set sts)
{
  Expression(sts);
  //optional
  match(",",sts); Expression(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::AssignmentStatement(Set sts)
{
  VariableAccessList(sts); match(":=",sts); ExpressionList(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ProcedureStatement(Set sts)
{
  match("call",sts);  ProcedureName(sts);
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::IfStatement(Set sts)
{
  match("if",sts); GuardedCommandList(sts); match("fi",sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::DoStatement(Set sts)
{
  match("do",sts); GuardedCommandList(sts); match("od",sts);
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::GuardedCommandList(Set sts)
{
  GuardedCommand(sts);
  //optional
  match("[]",sts); GuardedCommand(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::GuardedCommand(Set sts)
{
  Expression(sts); match("->",sts); StatementPart(sts);
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Expression(Set sts)
{
  PrimaryExpression(sts);
  //optional
  PrimaryOperator(sts); PrimaryExpression(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::PrimaryOperator(Set sts)
{
  match("&",sts);
  //or
  match("|",sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::PrimaryExpression(Set sts)
{
  SimpleExpression(sts);
  //1 or zero of the follwing
  RelationalOperator(sts); SimpleExpression(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::RelationalOperator(Set sts)
{
  match("<",sts);
  //or
  match("=",sts);
  //or
  match(">",sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::SimpleExpression(Set sts)
{
  //1 or zero of the following
  match("-",sts);
  //required
  Term(sts);
  //OPtional
  AddingOperator(sts); Term(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::AddingOperator(Set sts)
{
  match("+",sts);
  //or 
  match("-",sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Term(Set sts)
{
  Factor(sts);
  //optinal
  MultiplyingOperator(sts); Factor(sts);
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::MultiplyingOperator(Set sts)
{
  match("*",sts);
  //or
  match("/",sts);
  //or
  match("\\",sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Factor(Set sts)
{
  Constant(sts);
  //or
  VariableAccess(sts);
  //or
  match("(",sts); Expression(sts); match(")",sts);
  //or
  match("~",sts); Factor(sts);
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableAccess(Set sts)
{
  VariableName(sts);
  //one or zero of thefollowing
  IndexedSelector(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::IndexedSelector(Set sts)
{
  match("[", sts); Expression(sts); match("]", sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Constant(Set sts)
{
  Numeral(sts);
  //or
  BooleanSymbol(sts);
  //or
  ConstantName(sts);
  
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
  match("false", sts);
  //or
  match("true", sts);
  
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
void Parser::ProcedureName(Set sts)
{
  VariableName(sts);
  
  
  syntaxCheck(sts);
}