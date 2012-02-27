#include "parser.h"


Parser::Parser(Admin& adminObject)
{
  admin = &adminObject;
  debugflag = true;

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

void Parser::debug(string functionName, Set sts, Token nextToken) {
		if (debugflag == true) {
			//cout << "In function: " << functionName << ", Next Token:  " << nextToken.getLexeme() << ", Valid next Characters: " << sts.toString() << endl;		
			cout << "In function:" << functionName << endl;
		}
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

  cout<<"Syntax Error on token:" << lookAheadToken.getLexeme() << ""<< validNextCharacters.toString() << endl;
//	cout <<"Syntax Error on Token: " << lookAheadToken.getLexeme() << endl;
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
	debug(__func__, sts, lookAheadToken);
  Set* temp = new Set(".");
  
  Block(sts.munion(*temp)); 
  match(".", sts);
  
  syntaxCheck(sts);
  
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Block(Set sts)
{
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
  TypeSymbol(sts.munion(First::VariableDefinitionPart())); 
  VariableDefinitionPart(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableDefinitionPart(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  Set* temp = new Set("[");
  Set* temp2 = new Set("]");
  
  if(First::VariableList().isMember(lookAheadToken.getLexeme()))
  {
    VariableList(sts);
         }
  else if (lookAheadToken.getLexeme() == "array")
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
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
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
  debug(__func__, sts, lookAheadToken);
  match("proc",sts.munion(First::ProcedureName()).munion(First::Block())); 
  ProcedureName(sts.munion(First::Block())); 
  Block(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::StatementPart(Set sts)
{
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
    match("skip",sts);
    
    syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ReadStatement(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  match("read",sts.munion(First::VariableAccessList())); 
  VariableAccessList(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableAccessList(Set sts)
{
	debug(__func__, sts, lookAheadToken);
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
 debug(__func__, sts, lookAheadToken); 
  match("write",sts.munion(First::ExpressionList())); 
  ExpressionList(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ExpressionList(Set sts)
{
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
  Set *temp = new Set(":=");
  
  VariableAccessList(sts.munion(*temp).munion(First::ExpressionList())); 
  match(":=",sts.munion(First::ExpressionList())); 
  ExpressionList(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ProcedureStatement(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  match("call",sts.munion(First::ProcedureName()));  
  ProcedureName(sts);
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::IfStatement(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  Set *temp = new Set("fi");
  
  match("if",sts.munion(First::GuardedCommandList()).munion(*temp)); 
  GuardedCommandList(sts.munion(*temp)); 
  match("fi",sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::DoStatement(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  Set *temp = new Set("od");
  
  match("do",sts.munion(First::GuardedCommandList()).munion(*temp)); 
  GuardedCommandList(sts.munion(*temp)); 
  match("od",sts);
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::GuardedCommandList(Set sts)
{
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
  Set *temp = new Set("->");
  
  Expression(sts.munion(*temp).munion(First::StatementPart())); 
  match("->",sts.munion(First::StatementPart())); 
  StatementPart(sts);
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Expression(Set sts)
{
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
  Set constant = First::Constant();
  Set varacc = First::VariableAccess();
  Set *temp = new Set(")");
  bool perror = false;
    
    if (First::FactorName().isMember(lookAheadToken.getLexeme())) 
    {
        FactorName(sts);
    }
    else if (First::Constant().isMember(lookAheadToken.getLexeme()))
        Constant(sts);
  else if (lookAheadToken.getLexeme() == "(")
  {
    match("(",sts.munion(First::Expression()).munion(Set(")"))); 
    Expression(sts.munion(Set(")"))); 
    match(")",sts);
  }
  //or
  else if ( lookAheadToken.getLexeme() == "~")
  {
    match("~",sts.munion(First::Factor())); 
    Factor(sts);
  }
  else
  	perror = true;
  
  Set tempset = Set().munion(First::Constant()).munion(First::VariableAccess()).munion(Set("(")).munion(Set("~"));
  if (!tempset.isMember("") && perror)
  	syntaxError(sts);
  
  
  syntaxCheck(sts);
}


void Parser::FactorName(Set sts) {
    
    match("name", sts.munion(First::Constant()).munion(First::VariableAccess()));
    
    if (First::Constant().isMember(lookAheadToken.getLexeme())) {
     
        if(First::Numeral().isMember(lookAheadToken.getLexeme()))
        {
            Numeral(sts);
        }
        //or
        else if (First::BooleanSymbol().isMember(lookAheadToken.getLexeme()))
        {
            BooleanSymbol(sts);
        }
        
    }
    else if (First::IndexedSelector().isMember(lookAheadToken.getLexeme()))
        IndexedSelector(sts);
    
    syntaxCheck(sts);
    
}

/////////////////////////////////////////////////////////////////////////////
void Parser::VariableAccess(Set sts)
{
	debug(__func__, sts, lookAheadToken);
	
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
	debug(__func__, sts, lookAheadToken);
  Set *temp = new Set("]");
  
  match("[", sts.munion(First::Expression()).munion(*temp)); 
  Expression(sts.munion(*temp)); 
  match("]", sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::Constant(Set sts)
{
	debug(__func__, sts, lookAheadToken);
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
	debug(__func__, sts, lookAheadToken);
  match("num", sts);//TODO change token creation to make lexeme of a number = "num"
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::BooleanSymbol(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  if(lookAheadToken.getLexeme() == "false")
  {
    match("false", sts);
  }
  //or
  else if ( lookAheadToken.getLexeme() == "true")
  {
    match("true", sts);
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ConstantName(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  //token must be a reserved word TODO - might not be a reserved word.
  VariableName(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableName(Set sts)
{
  debug(__func__, sts, lookAheadToken);
	  //token must be a user defined word
  match("name", sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ProcedureName(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  VariableName(sts);
  
  
  syntaxCheck(sts);
}