/**
 * @brief The definition file for the Parser component object.
 *
 * @file parser.cpp
 * 
 * The Parser is responsible for properly syntactically verifying that the
 * input is correct, and (virtually) forming a syntax and grammatical tree
 * from the scanned input.
 *
 * It accomplishes this by linking back to the Scanner through the Admin,
 * using it to grab a given Token, then intelligently proceeding to the next
 * set of proper steps in the Grammatical structure of the PL Language. 
 *
 * Each of the following functions represents a grammatical rule and 
 * fulfills the corresponding set of productions via function calls;
 * a recursive descent stack is created implicitly by the function calls,
 * and the "tree" of function calls is the end logical representation
 * of the Parse tree for the grammar.
 *
 * We use the output function debug() with the C++ macro "__func__" to output
 * the current Parsing function - that is, which node in the descent tree our
 * parsing and scanning is currently in.
 *
 * A SET is an object containing a vector of lexemic strings. @see set.cpp
 * The Set "sts" that is passed between objects is the "stop set", which functions 
 * as a sentinel set indicating the valid next characters; if we see a lexeme that
 * is not in the (sts) Stop Set, we stop (generate an error).
 * 
 * @see /documentation/
 *
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to February 29th, 2011
 **/

#include "parser.h"

/**
 * @brief Constructor for Parser object class
 * 
 * Binds the external admin object locally, controls the debug
 * status and sets the initial lookahead token/scanner var position.
 *
 * @param adminObject a reference to the parent Admin object.
 */
Parser::Parser(Admin& adminObject, SymbolTable& symTable)
{
  admin = &adminObject;
  table = &symTable;
  debugflag = true;
  errorCount = 0;
  blocktable = new BlockTable(*table);
  prevMatch[0] = "";
  prevMatch[1] = "";
  prevMatch[2] = "";
  prevMatch[3] = "";
  
  lookAheadToken = nextToken();
}

/** Base destructor. */
Parser::~Parser()
{
}

/**
 * @brief The main Runner for the Parser object.
 * 
 * Sets the $ marker for the file as a stopset value.
 * This ensures that $ does not get recognized as a syntax error
 * and that any matches that happen to include a $ will be correct.
 * The initial grammar prod., Program, is parsed (executed with sts).
 */
void Parser::run()
{ 
  Set* temp = new Set("$");
  //send in money sign
  
  this->Program(*temp);
  cout << "\nParsing completed.\n";
  if(errorCount > 0)
    cout << "\nParsing Errors Found: " << errorCount << endl;
  else
    cout << "\nNo Parsing errors found. \n";
}

/**
 * @brief Parsing Debug information output function.
 * 
 * Outputs the current parsing information as a form of
 * debugging information.
 * The function name is grabbed from the __func__ C++ macro.
 *
 * @param functionName The name of the function caller.
 * @param sts The current stack's stop set.
 * @param nextToken The next token being parsed.
 */
void Parser::debug(string functionName, Set sts, Token nextToken) 
{
  
  if (debugflag == true) 
  {
    //cout << "In function: " << functionName << ", Next Token:  " << nextToken.getLexeme() << ", Valid next Characters: " << sts.toString() << endl;		
    cout << "In function: " << functionName << "\t\t to parse line " << admin->getLineNumber() << ", column " << admin->getColumnNumber() << endl;
  }
}


/**
 * @brief Function that advances the scanner until a valid input is found.
 * Loops the tokenizer until a non-bad/non-newline/non-comment token is
 * found.
 * Returns a token, possibly blank.
 */
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

/**
 * @brief The Parser's lexeme Matching function
 * 
 * 
 * @param matchMe A lexeme that we wish to check for a match for validity.
 * @param validNextCharacters A set of characters that would be a "valid" next input.
 */
void Parser::match(string matchMe, Set validNextCharacters)
{
  /* Preliminary debug functions.
  cout << "\nTesting matchMe: " << matchMe << " and la: " << lookAheadToken.getLexeme() << "\n" << validNextCharacters.toString() << "\n" << endl;
  char a;
  cin >> a;
  */
  debug(__func__, validNextCharacters,lookAheadToken);
  //cout << "\n\nTrying to match lookahead " << lookAheadToken.getLexeme() << " and match token " << matchMe << " and valid chars " << validNextCharacters.toString();
  
	bool error = false;

  if (lookAheadToken.getLexeme() == matchMe) 
  {
  	
  	if(lookAheadToken.getType() == IDENTIFIER) {
  		
  	if (prevMatch[0] == "const") {
	  error = blocktable->define(lookAheadToken.getValue(), CONSTANT, UNIVERSAL );
		prevID = lookAheadToken.getValue();
	}
	  
	else if (prevMatch[0] == "array") {
		
		if (prevMatch[1] == "integer")
		 error = blocktable->define(lookAheadToken.getValue(), ARRAY, INTEGER);
		else if (prevMatch[1] == "Boolean")
		  error =blocktable->define(lookAheadToken.getValue(), ARRAY, BOOLEAN);
		
	}
  	else if (prevMatch[0] == "Boolean" && lookAheadToken.getLexeme() != "array")
	  error = blocktable->define(lookAheadToken.getValue(), VARIABLE, BOOLEAN);
		
  	else if (prevMatch[0] == "integer" && lookAheadToken.getLexeme() != "array")
	  error = blocktable->define(lookAheadToken.getValue(), VARIABLE, INTEGER);
	
	else if (prevMatch[0] == "proc")
	  error = blocktable->define(lookAheadToken.getValue(), PROCEDURE, UNIVERSAL);
	
	
  	} 
  	
	prevToken = lookAheadToken;
    lookAheadToken = nextToken();
    
  }
  else
  {
    
    syntaxError(validNextCharacters);
  }
  //cout << "\nHere's the debug data.\n MatchMe is " << matchMe << " and the valset is " << validNextCharacters.toString() << endl;
  syntaxCheck(validNextCharacters);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::syntaxError( Set validNextCharacters)
{
  ++errorCount;
  cout << "\nSyntax Error on token: " << lookAheadToken.getLexeme() << " with LA " << currentToken.getLexeme() <<  ". Valid set members are: " << validNextCharacters.toString() 
       << "\t line/col " << admin->getLineNumber() << " " << admin->getColumnNumber() << endl;

	//Reset Matched variable to nothing
	resetPrevMatches();

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


/**
 *
 * The following is a set of Production-Rule functions. Each one has a specific
 * speciality in regards to how they treat their input and production rules,
 * but on the whole they follow a key process:
 *
 * 1) Output the function name and data.
 * 2) Choose a production rule to follow based off of the lookahead
 * 3) Try to Match any prerequisite statements or rules
 * 4) Append any valid lexemes to the StopSet sts
 * 5) Choose a production rule and follow it (a non-terminal)
 * 6) Repeat ~3-5 unless an error is found
 * 7) Attempt to match the post-requisite statement/lexeme/rule.
 * 8) Check the validity of the lookahead in regards to the stopset sts
 *
 * This implicitly maintains a "stack" of unique stopsets at each function (production)
 * level, These are allocated on the application stack.
 *
 * Thus, we need to be wary of Stack Overflows when using the recursive parser.
 *
 */

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
  blocktable->newBlock();
  DefinitionPart(sts.munion(First::StatementPart()).munion(*temp)); 
  StatementPart (sts.munion(*temp)); 
  blocktable->printAllBlocks();
  match         ("end", sts);
  blocktable->endBlock();
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
	resetPrevMatches();
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
  int tokenID = 0;
  int constVal = 0;
  mType type = UNIVERSAL;
  
  match("const", sts.munion(First::ConstantName()).munion(*temp).munion(First::Constant())); 
  tokenID = lookAheadToken.getValue();
  ConstantName(sts.munion(*temp).munion(First::Constant())); 
  match("=", sts.munion(First::Constant())); 
	
	if (lookAheadToken.getLexeme() == "num") {
  		constVal = lookAheadToken.getValue();
		type = INTEGER;
	}
	else if (lookAheadToken.getLexeme() == "true" || lookAheadToken.getLexeme() == "false") {
		type = BOOLEAN;
		if (lookAheadToken.getLexeme() == "true")
			constVal = 1;
		else {
			constVal = 0;
		}
	} else if (lookAheadToken.getLexeme() == "name") {
		TableEntry* tbl;
		if (blocktable->search(lookAheadToken.getValue(), tbl)) {
			
			if (tbl->okind == CONSTANT) {
				
				constVal = tbl->value;
				
			}
		} else {
			cout << "Type Mismatch: expected constant" << endl;
		}
		
	}
		
	
  Constant(sts);
	blocktable->define(tokenID, CONSTANT, type, 0, constVal);
  
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
    prevMatch[0] = "array";
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
	prevMatch[0] = prevMatch[1] = "integer";
  }
  else if (lookAheadToken.getLexeme() == "Boolean")
  {
    match("Boolean", sts);
	prevMatch[0] = prevMatch[1] = "Boolean";
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
  prevMatch[0] = "proc";
  ProcedureName(sts.munion(First::Block())); 
	resetPrevMatches();
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
    match(";",sts.munion(First::Statement()));
	resetPrevMatches();
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
  Set procedure = First::ProcedureStatement() ;
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
 
  //
  // Do we need this still? It seems like it was supposed to be used in those MUNIONS
    //Set *temp = new Set(")");
  //
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


void Parser::FactorName(Set sts) 
{
	debug(__func__, sts, lookAheadToken);
  match("name", sts.munion(First::Constant()).munion(First::VariableAccess()));
    
  if (First::Constant().isMember(lookAheadToken.getLexeme())) 
  {
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
	//blocktable->redefineValue(prevID, prevToken.getLexeme());
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


void Parser::resetPrevMatches() {
	prevMatch[0] = "";
	prevMatch[1] = "";
	prevMatch[2] = "";
	prevMatch[3] = "";
}
