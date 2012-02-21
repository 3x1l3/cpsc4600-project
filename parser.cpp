#include "parser.h"


Parser::Parser(Admin& adminObject)
{
  admin = &adminObject;
  

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
    match(";", sts.munion(First::Definition()));//aded in first of definition so the while loop can keep going.
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
  TypeSymbol(sts.munion(First::VariableDefinitionPart())); 
  VariableDefinitionPart(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableDefinitionPart(Set sts)
{
  Set* temp = new Set("[");
  Set* temp2 = new Set("]");
  
  if(First::VariableList().isMember(lookAheadToken.getLexeme()))
  {
    VariableList(sts);
  }
  else if (lookAheadToken.getLexeme() == "array");
  {
    match("array",sts.munion(First::VariableList()).munion(*temp).munion(First::Constant()).munion(*temp2)); 
    VariableList(sts.munion(*temp).munion(First::Constant()).munion(*temp2)); 
    match("[",sts.munion(First::Constant()).munion(*temp2)); 
    Constant(sts.munion(*temp2)); 
    match("]",sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::TypeSymbol(Set sts)
{
  if(lookAheadToken.getLexeme() == "integer")
  {
    match("integer", sts);
  }
  else if (lookAheadToken.getLexeme() == "Boolean")
  {
    match("Boolean", sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableList(Set sts)
{
  Set *temp = new Set(",");
  
  
  VariableName(sts.munion(*temp).munion(First::VariableName()));
  //optional part
  while(temp->isMember(lookAheadToken.getLexeme()))
  {
    match(",",sts.munion(First::VariableName())); 
    VariableName(sts.munion(*temp)); //here i added in the comma symbol, so the loop can continue if lookahead is another comma
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ProcedureDefintion(Set sts)
{
  
  match("proc",sts.munion(First::ProcedureName()).munion(First::Block())); 
  ProcedureName(sts.munion(First::Block())); 
  Block(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::StatementPart(Set sts)
{
  Set *temp = new Set(";");
  
  //optional
  while(First::Statement().isMember(lookAheadToken.getLexeme()))
  {
    Statement(sts.munion(*temp)); 
    match(";",sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Statement(Set sts)
{
  Set empty = First::EmptyStatement();
  Set read = First::ReadStatement();
  Set write = First::WriteStatement();
  Set assignment = First::AssignmentStatement();
  Set procedure = First::ProcedureDefinition() ;
  Set iff = First::IfStatement();
  Set doo = First::DoStatement();
  
  
  if(empty.isMember(lookAheadToken.getLexeme()))
  {
    EmptyStatement(sts);
  }
  else if(read.isMember(lookAheadToken.getLexeme()))
  {
    ReadStatement(sts);
  }
  else if(write.isMember(lookAheadToken.getLexeme()))
  {
    WriteStatement(sts);
  }
  else if(assignment.isMember(lookAheadToken.getLexeme()))
  {
    AssignmentStatement(sts);
  }
  else if(procedure.isMember(lookAheadToken.getLexeme()))
  {
    ProcedureStatement(sts);
  }
  else if(iff.isMember(lookAheadToken.getLexeme()))
  {
    IfStatement(sts);
  }
  else if(doo.isMember(lookAheadToken.getLexeme()))
  {
    DoStatement(sts);
  }
  
  
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
  match("read",sts.munion(First::VariableAccessList())); 
  VariableAccessList(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableAccessList(Set sts)
{
  Set *temp = new Set(",");
  
  VariableAccess(sts.munion(*temp).munion(First::VariableAccess()));
  //optional
  while(lookAheadToken.getLexeme() == ",")
  {
    match(",",sts.munion(First::VariableAccess())); 
    VariableAccess(sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::WriteStatement(Set sts)
{
  
  match("write",sts.munion(First::ExpressionList())); 
  ExpressionList(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ExpressionList(Set sts)
{
  Set *temp = new Set(",");
  
  Expression(sts.munion(*temp).munion(First::Expression()));
  //optional
  while(lookAheadToken.getLexeme() == ",")
  {
    match(",",sts.munion(First::Expression())); 
    Expression(sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::AssignmentStatement(Set sts)
{
  Set *temp = new Set(":=");
  
  VariableAccessList(sts.munion(*temp).munion(First::ExpressionList())); 
  match(":=",sts.munion(First::ExpressionList())); 
  ExpressionList(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ProcedureStatement(Set sts)
{
  
  match("call",sts.munion(First::ProcedureName()));  
  ProcedureName(sts);
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::IfStatement(Set sts)
{
  Set *temp = new Set("fi");
  
  match("if",sts.munion(First::GuardedCommandList()).munion(*temp)); 
  GuardedCommandList(sts.munion(*temp)); 
  match("fi",sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::DoStatement(Set sts)
{
  Set *temp = new Set("od");
  
  match("do",sts.munion(First::GuardedCommandList()).munion(*temp)); 
  GuardedCommandList(sts.munion(*temp)); 
  match("od",sts);
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::GuardedCommandList(Set sts)
{
  Set *temp = new Set("[]");
  
  GuardedCommand(sts.munion(*temp).munion(First::GuardedCommand()));
  //optional
  while(lookAheadToken.getLexeme() == "[]")
  {
    match("[]",sts.munion(First::GuardedCommand())); 
    GuardedCommand(sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::GuardedCommand(Set sts)
{
  Set *temp = new Set("->");
  
  Expression(sts.munion(*temp).munion(First::StatementPart())); 
  match("->",sts.munion(First::StatementPart())); 
  StatementPart(sts);
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Expression(Set sts)
{
  PrimaryExpression(sts.munion(First::PrimaryOperator()).munion(First::PrimaryExpression()));
  //optional
  while(First::PrimaryOperator().isMember(lookAheadToken.getLexeme()))
  {
    PrimaryOperator(sts.munion(First::PrimaryExpression())); 
    PrimaryExpression(sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::PrimaryOperator(Set sts)
{
  if(lookAheadToken.getLexeme() == "&")
  {
    match("&",sts);
  }
  //or
  else if (lookAheadToken.getLexeme() == "|")
  {
    match("|",sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::PrimaryExpression(Set sts)
{
  SimpleExpression(sts.munion(First::RelationalOperator()).munion(First::SimpleExpression()));
  //1 or zero of the follwing
  if(First::RelationalOperator().isMember(lookAheadToken.getLexeme()))
  {
    RelationalOperator(sts.munion(First::SimpleExpression())); 
    SimpleExpression(sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::RelationalOperator(Set sts)
{
  if(lookAheadToken.getLexeme() == "<")
  {
    match("<",sts);
  }
  //or
  else if(lookAheadToken.getLexeme() == "=")
  {
    match("=",sts);
  }
  //or
  else if(lookAheadToken.getLexeme() == ">")
  {
    match(">",sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::SimpleExpression(Set sts)
{
  //1 or zero of the following
  if(lookAheadToken.getLexeme() == "-")
  {
    match("-",sts.munion(First::Term()).munion(First::AddingOperator()));
  }
  
  //required
  Term(sts.munion(First::AddingOperator()).munion(First::Term()));
  
  //Optional
  while(First::AddingOperator().isMember(lookAheadToken.getLexeme()))
  {
    AddingOperator(sts.munion(First::Term())); 
    Term(sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::AddingOperator(Set sts)
{
  if(lookAheadToken.getLexeme() == "+")
  {
    match("+",sts);
  }
  //or
  else if(lookAheadToken.getLexeme()=="-")
  {
    match("-",sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Term(Set sts)
{
  Factor(sts.munion(First::MultiplyingOperator()).munion(First::Factor()));
  //redundant check for if it is in factor?
  //casue below we have a while statement that chceks only for if it is in multilpfying opertor.
  //so program could come back here thinking first of factor is correct, but then we dont use it here,
  //only use first of multpiying operator......? TODO
  
  
  //optinal
  while(First::MultiplyingOperator().isMember(lookAheadToken.getLexeme()))
  {
    MultiplyingOperator(sts.munion(First::Factor()));  
    Factor(sts);
  }
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::MultiplyingOperator(Set sts)
{
  if(lookAheadToken.getLexeme() == "*")
  {  
    match("*",sts);
  }
  //or
  if(lookAheadToken.getLexeme() == "/")
  {
    match("/",sts);
  }
  //or
  if(lookAheadToken.getLexeme() == "\\")
  {
    match("\\",sts);
  }
  
  
  syntaxCheck(sts);
}
///////////////////////////////////////////////////////////////////////////// 
void Parser::Factor(Set sts)
{
  Set constant = First::Constant();
  Set varacc = First::VariableAccess();
  Set *temp = new Set(")");
  
  
  if(constant.isMember(lookAheadToken.getLexeme()))
  {
    Constant(sts);
  }
  //or
  else if(varacc.isMember(lookAheadToken.getLexeme()))
  {
    VariableAccess(sts);
  }
  //or
  else if (lookAheadToken.getLexeme() == "(")
  {
    match("(",sts.munion(First::Expression()).munion(*temp)); 
    Expression(sts.munion(*temp)); 
    match(")",sts);
  }
  //or
  else if ( lookAheadToken.getLexeme() == "~")
  {
    match("~",sts.munion(First::Factor())); 
    Factor(sts);
  }
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableAccess(Set sts)
{
  Set indsel = First::IndexedSelector();
  
  VariableName(sts.munion(First::IndexedSelector()));
  //one or zero of thefollowing
  if(indsel.isMember(lookAheadToken.getLexeme()))
  {
    IndexedSelector(sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::IndexedSelector(Set sts)
{
  Set *temp = new Set("]");
  
  match("[", sts.munion(First::Expression()).munion(*temp)); 
  Expression(sts.munion(*temp)); 
  match("]", sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Constant(Set sts)
{
  Set num = First::Numeral();
  Set bol = First::BooleanSymbol();
  Set con = First::ConstantName();
  
  if(num.isMember(lookAheadToken.getLexeme()))
  {
    Numeral(sts);
  }
  //or
  else if (bol.isMember(lookAheadToken.getLexeme()))
  {
    BooleanSymbol(sts);
  }
  //or
  else if (con.isMember(lookAheadToken.getLexeme()))
  {
    ConstantName(sts);
  }
  
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