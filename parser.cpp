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
  debugflag = false;
  errorCount = 0;
  numberOfScopeTypeErrors = 0;
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
  cout<<endl;
  cout<<"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~"<<endl;
  cout << "Parsing completed."<<endl;;
  if(errorCount > 0)
    cout << "Parsing Errors Found: " << errorCount << endl;
  else
    cout << "No Parsing errors found. "<<endl;;
  
  
    cout << "Scope and Type check Completed."<<endl;
  if(numberOfScopeTypeErrors > 0)
    cout << "Scope and Type Check Errors Found: " << numberOfScopeTypeErrors << endl;
  else
    cout << "No Scope or Type check errors found. "<<endl;;
  
  
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
  debug(__func__, validNextCharacters,lookAheadToken);
  //cout<<"Matching symbol : " <<lookAheadToken.getLexeme()<<endl;
	
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
  ++errorCount;
  cout << "\nSyntax Error on token: " << lookAheadToken.getLexeme() << " with LA " << currentToken.getLexeme() <<  ". Valid set members are: " << validNextCharacters.toString() 
       << "\t line/col " << admin->getLineNumber() << " " << admin->getColumnNumber() << endl;


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
  blockTypeStack.push(DEFINITIONPART);
  
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
  blockTypeStack.pop();
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
			bool error = false;
		TableEntry tbl = blocktable->find(lookAheadToken.getValue(), error);
	
		if (!error) 
		{	
			if (tbl.okind == CONSTANT) 
			{	
				constVal = tbl.value;
				type = tbl.otype;
			} else 
			{
			  cout << "Type Mismatch: expected constant" << endl;
			  numberOfScopeTypeErrors++;
			  cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
			}
		} 
		else 
		{ 
		  cout << "Error: constant not defined" << endl;
		  numberOfScopeTypeErrors++;
		  cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
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
  mType type = TypeSymbol(sts.munion(First::VariableDefinitionPart())); 
  VariableDefinitionPart(sts, type);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::VariableDefinitionPart(Set sts, mType type)
{
	debug(__func__, sts, lookAheadToken);
  Set* temp = new Set("[");
  Set* temp2 = new Set("]");
  
  if(First::VariableList().isMember(lookAheadToken.getLexeme()))
  {
    VariableList(sts, type, VARIABLE);
  }
  else if (lookAheadToken.getLexeme() == "array")
  {
	bool error = false;
	TableEntry entry;
	int constid;
	vector<int> arrayIDs;
	
    match("array",sts.munion(First::VariableList()).munion(*temp).munion(First::Constant()).munion(*temp2)); 
    
    arrayIDs = VariableList(sts.munion(*temp).munion(First::Constant()).munion(*temp2), type, ARRAY); 
    match("[",sts.munion(First::Constant()).munion(*temp2)); 
    
	constid = lookAheadToken.getValue();
	//for some reason, this returns UNIVERSAL no matter what type the actual item in between the square brackets is...
	//very strange... but if we access entry after the call to entry = blocktable->find(constid, error); below,
	//we can accuratly get the type of the object by blocktable->convertType(entry.otype)
	//so for sake of easiness i will just use the entry type and compare it to the type paramter passed in to this function.
	//this will allow us to check on declaration of a array, if the size variable is integer as it should be, or boolean which is error
	Constant(sts.munion(*temp2)); 
	
	match("]",sts);

	entry = blocktable->find(constid, error);
	if(entry.otype != INTEGER)
	{
	  cout<<"Array Size variable is of non-Integer type.  Trying to create array size with type of "<<blocktable->convertType(entry.otype)<<endl;
	  numberOfScopeTypeErrors++;
	  cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
	}
	
	if (!error) 
	{
	  for(int i=0; i<(int)arrayIDs.size(); i++) 
	  {
		  blocktable->redefineSize(arrayIDs.at(i), entry.value);
	  }
	} 
	else 
	{
		  
		  cerr << "Constant undefined for use in array definition" << endl;
		  numberOfScopeTypeErrors++;
		  cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
		  
	}
	
	
	
  }
  
  syntaxCheck(sts);
  

}
/////////////////////////////////////////////////////////////////////////////
mType Parser::TypeSymbol(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  if(lookAheadToken.getLexeme() == "integer")
  {
	
    match("integer", sts);
	
	return INTEGER;
  }
  else if (lookAheadToken.getLexeme() == "Boolean")
  {
    match("Boolean", sts);
	return BOOLEAN;
  } else {
      return UNIVERSAL;
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
vector<int> Parser::VariableList(Set sts, mType type, Kind kind)
{
	
	vector<int> tokenIDs;
	debug(__func__, sts, lookAheadToken);
  Set *temp = new Set(",");
  string name = "";
  int id = 0;
	bool error = false;
  name = lookAheadToken.getLexeme();
	id = lookAheadToken.getValue();  
	
	VariableName(sts.munion(*temp).munion(First::VariableName()));
	
	error = blocktable->define(id, kind, type);
	
	if (error)
	{
	  cout << "Multiple "<<blocktable->convertKind(kind)<< " declaration: " 
	       << blocktable->table->getAttributeWhere(id, "ID", "lexeme") << endl;
	  numberOfScopeTypeErrors++;
	  cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
	}
	else 
	{
	   tokenIDs.push_back(id);
	}
			
	  //optional part
	error = false;

	while(temp->isMember(lookAheadToken.getLexeme()))
	{
		
	  match(",",sts.munion(First::VariableName())); 
	  name = lookAheadToken.getLexeme();
	  id = lookAheadToken.getValue(); 
		    
	  VariableName(sts.munion(*temp)); //here i added in the comma symbol, so the loop can continue if lookahead is another comma
	  error = blocktable->define(id, kind, type);
	  if (error)
	  {
	     cout << "Multiple "<<blocktable->convertKind(kind)<< " declaration: " 
	          << blocktable->table->getAttributeWhere(id, "ID", "lexeme") << endl;
		  numberOfScopeTypeErrors++;
		  cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
	  }
	  else 
	  {
	    tokenIDs.push_back(id);
	  }
	}
	  
	  syntaxCheck(sts);
	return tokenIDs;
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ProcedureDefintion(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  
  match("proc",sts.munion(First::ProcedureName()).munion(First::Block())); 
  blocktable->define(lookAheadToken.getValue(), PROCEDURE, UNIVERSAL);
  ProcedureName(sts.munion(First::Block())); 
  Block(sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::StatementPart(Set sts)
{
  blockTypeStack.push(STATEMENTPART);
	debug(__func__, sts, lookAheadToken);
  Set *temp = new Set(";");
  
  //optional
  while(First::Statement().isMember(lookAheadToken.getLexeme()))
  {
    Statement(sts.munion(*temp)); 
    match(";",sts.munion(First::Statement()));
	resetPrevMatches();
  }
  blockTypeStack.pop();
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
vector<mType> Parser::VariableAccessList(Set sts)
{
	vector<mType> varTypes; 
        debug(__func__, sts, lookAheadToken);
        Set *temp = new Set(",");
 
	varTypes.push_back(VariableAccess(sts.munion(*temp).munion(First::VariableAccess())));
	
        //optional////////////
	while(lookAheadToken.getLexeme() == ",")
	{
	  match(",",sts.munion(First::VariableAccess())); 
	  
	  varTypes.push_back(VariableAccess(sts.munion(*temp)));
	}
	
	syntaxCheck(sts);
        return varTypes;
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
vector<mType> Parser::ExpressionList(Set sts)
{
  vector<mType> types;
  debug(__func__, sts, lookAheadToken);
  Set *temp = new Set(",");
  
  
  types.push_back(Expression(sts.munion(*temp).munion(First::Expression())));
  
////////////////////////////////////////////////////////////////////////////////

  
  //optional
  while(lookAheadToken.getLexeme() == ",")
  {
    match(",",sts.munion(First::Expression()));     
    types.push_back(Expression(sts.munion(*temp)));
  }
      
      
  syntaxCheck(sts);
  
//Jordan's Debug Info - Please Leave - Mar 31st 2012  
//   for(int i = 0 ; i < (int)types.size(); i++)
//   {
//     cout<<"####################ExprList(printOutAllVector)="<<blocktable->convertType(types.at(i))<<endl;
//   }
  return types;
}
/////////////////////////////////////////////////////////////////////////////
void Parser::AssignmentStatement(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  Set *temp = new Set(":=");
  
  vector <mType> val;
  vector <mType> el;
  
  val = VariableAccessList(sts.munion(*temp).munion(First::ExpressionList())); 
  
  match(":=",sts.munion(First::ExpressionList())); 
  
  el = ExpressionList(sts);
  
  if(val.size() == el.size())
  {
    
    //add in better error print out here
    bool allOk = true;
    int indexSelect;
    
    for(int i = 0; i < (int) val.size(); i++)
    {
      if(val.at(i) != el.at(i))
      {
	allOk = false;
	indexSelect = i;
      }
//       if(val.at(i) == UNIVERSAL)
//       {
// 	cout<<"Error - Trying to redefine a constant variable. - variable # " <<i<<" on right hand side of assignment statement"<<endl;
// 	numberOfScopeTypeErrors++;
//       }
      //Jordan's debuging info - Please Leave - Mar 31 2012
      //cout<<"66666666666666666666666666here"<<endl;
      //cout<<"66666666666666666666666666variable acces list : "<<blocktable->convertType(val.at(i))<<endl;
      //cout<<"66666666666666666666666666expression list : "<<blocktable->convertType(el.at(i))<<endl;
    }
    if (!allOk)
    {
      cout<<"Assignment type mismatch at place (from 0) " <<indexSelect<<". Trying to assign " << blocktable->convertType(val.at(indexSelect))<<" to "<< blocktable->convertType(el.at(indexSelect)) <<endl;
      numberOfScopeTypeErrors++;
      cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
    }
  }
  else
  {
    cout<<"Unbalanced assignment statement (number of variable pairs is unequal)."<<endl;
    numberOfScopeTypeErrors++;
    cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::ProcedureStatement(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  match("call",sts.munion(First::ProcedureName()));
  
  int id = lookAheadToken.getValue();
  bool error = false;
  TableEntry entry;
  
  ProcedureName(sts);
  
  entry = blocktable->find(id, error);
  
  if(entry.okind != PROCEDURE)
  {
    cout<<"Invalid procedure call to type of "<<blocktable->convertKind(entry.okind)<<"-"<<blocktable->convertType(entry.otype)<<endl;
    numberOfScopeTypeErrors++;
    cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
  }
  if(error)
  {
    cout<<"Undefined Procedure Call - Unreferenced Procedure Name."<<endl;
    numberOfScopeTypeErrors++;
    cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
  }
  
  
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
    GuardedCommand(sts.munion(*temp));
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
void Parser::GuardedCommand(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  Set *temp = new Set("->");
  
  if (Expression(sts.munion(*temp).munion(First::StatementPart())) != BOOLEAN)
  {
    //Need a better print out here - JP Mar31 2012
    cout << "Compound guarded command statement (entry conditions for while, do, for, if) did not evaluate to boolean." << endl;
    numberOfScopeTypeErrors++;
    cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
  }
  match("->",sts.munion(First::StatementPart())); 
  StatementPart(sts);
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
mType Parser::Expression(Set sts)
{
  vector<mType> localTypes;
  
	debug(__func__, sts, lookAheadToken);
  localTypes.push_back(PrimaryExpression(sts.munion(First::PrimaryOperator()).munion(First::PrimaryExpression())));
  
  //optional
  while(First::PrimaryOperator().isMember(lookAheadToken.getLexeme()))
  {
    PrimaryOperator(sts.munion(First::PrimaryExpression())); 
    localTypes.push_back(PrimaryExpression(sts));
  }
  

  
  syntaxCheck(sts);
  if((int)localTypes.size() == 1)
  {
    //Jordan's debuggin info - Please Leave - Mar 31 2012
    //cout<<"********************ExprReturn(VecSize1)="<<blocktable->convertType(localTypes.at(0))<<endl;
    return localTypes.at(0);
  }
  
  for(int i=0; i < (int)localTypes.size(); i++) 
  {
    if (localTypes.at(i) != BOOLEAN)
    {  
      //leave - JP march 31
      //cout<<"********************ExprReturn(SomeNotBoolean)="<<blocktable->convertType(UNIVERSAL)<<endl;
      return UNIVERSAL;
    }
  }
  //leave - JP march 31
  //cout<<"********************ExprReturn(AllBoolean)="<<blocktable->convertType(BOOLEAN)<<endl;
  return BOOLEAN;
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
mType Parser::PrimaryExpression(Set sts)
{
    mType type1, type2;
	debug(__func__, sts, lookAheadToken);
	bool type2found = false;
 
  if (lookAheadToken.getLexeme() == "~" || lookAheadToken.getLexeme() == "(") 
  {
      type1 = Factor(sts);
      //maybe have to do something here too TODO - JP march 31 2012
      if (type1 == BOOLEAN)
	return type1;
      else
	return UNIVERSAL;
  } 
  else 
  {
	  
  type1 = SimpleExpression(sts.munion(First::RelationalOperator()).munion(First::SimpleExpression()));
  //1 or zero of the follwing
  if(First::RelationalOperator().isMember(lookAheadToken.getLexeme()))
  {
    type2found = true;
    RelationalOperator(sts.munion(First::SimpleExpression())); 
    type2 = SimpleExpression(sts);
    
  }
  
  syntaxCheck(sts);
  
  if(type2found)
  {
    if (type1 == INTEGER && type2 == INTEGER)
    {
      //JP debug - leave - march 31 2012
      //cout<<"******************************************PrimExprReturn(type1&type2found)="<<blocktable->convertType(BOOLEAN)<<endl;
      return BOOLEAN;
    }
    else
    {
      //jp debug leave mar 31 2012
      //cout<<"******************************************PrimExprReturn(noneInteger)="<<blocktable->convertType(UNIVERSAL)<<endl;
      
      //primary expression did not evaulate to boolean type (meaning all the elements where not of one type)
      return UNIVERSAL;
    }
  }
  
  if(!type2found)
  {
    if(type1 == INTEGER) 
    {
      //jp - leave - mar 31
      //cout<<"******************************************PrimExprReturn(type1found)="<<blocktable->convertType(INTEGER)<<endl;
      return INTEGER;
    }
    else if (type1 == BOOLEAN)
    {
      //jp - leave - mar 31
      //cout<<"******************************************PrimExprReturn(noneInteger)="<<blocktable->convertType(BOOLEAN)<<endl;
      return BOOLEAN;
    }
    else
    {
      //jp - leave - mar 31
      //cout<<"******************************************PrimExprReturn(noneInteger)="<<blocktable->convertType(UNIVERSAL)<<endl;
      return UNIVERSAL;
    }
  }
  
  
  
  
  }
  
  //jp leave - mar 31
  //cout<<"***************************PrimExprReturn(defaultCaseAtBottom)="<<blocktable->convertType(UNIVERSAL)<<endl;
  return UNIVERSAL;
  
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
mType Parser::SimpleExpression(Set sts)
{
  vector<mType> localtypes;
  debug(__func__, sts, lookAheadToken);
  //1 or zero of the following
  if(lookAheadToken.getLexeme() == "-")
  {
    match("-",sts.munion(First::Term()).munion(First::AddingOperator()));
  }
  

  //required
  localtypes.push_back(Term(sts.munion(First::AddingOperator()).munion(First::Term())));
  
  //Optional
  while(First::AddingOperator().isMember(lookAheadToken.getLexeme()))
  {
    AddingOperator(sts.munion(First::Term())); 
 
    localtypes.push_back(Term(sts));
  }
  

  syntaxCheck(sts);
  
  //this is to check when we only see one factor here.  then we just return that one factor's type,
  //instead of checking if they are all INTEGER, casue if there is only one, it doesnt have to be just INTEGER
  if((int)localtypes.size() == 1)
  {
    return localtypes.at(0);
  }
  
  for(int i=0; i < (int)localtypes.size(); i++) {
   if (localtypes.at(i) != INTEGER)
     //if we return universal, we have seen more than one term, and therefore need all of them to be
   //of integer type to make the adding operator valid.  AddOp not defined for boolean types in PL
     return UNIVERSAL;
  }
  return INTEGER;
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
mType Parser::Term(Set sts)
{
  vector<mType> localtypes;
	debug(__func__, sts, lookAheadToken);
 localtypes.push_back( Factor(sts.munion(First::MultiplyingOperator()).munion(First::Factor())));
  //redundant check for if it is in factor?
  //casue below we have a while statement that chceks only for if it is in multilpfying opertor.
  //so program could come back here thinking first of factor is correct, but then we dont use it here,
  //only use first of multpiying operator......? TODO
  
  
  //optinal
  while(First::MultiplyingOperator().isMember(lookAheadToken.getLexeme()))
  {
   MultiplyingOperator(sts.munion(First::Factor()));  
   localtypes.push_back( Factor(sts));
  }
  
  
  syntaxCheck(sts);
  
  //this is to check when we only see one factor here.  then we just return that one factor's type,
  //instead of checking if they are all INTEGER, casue if there is only one, it doesnt have to be just INTEGER
  if((int)localtypes.size() == 1)
  {
    return localtypes.at(0);
  }
  
    for(int i=0; i < (int)localtypes.size(); i++) {
   if (localtypes.at(i) != INTEGER)
    //if we return universal, we have seen more than one term, and therefore need all of them to be
   //of integer type to make the multiplying operator valid.  MulOp not defined for boolean types in PL
     return UNIVERSAL;
  }
  return INTEGER;
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
mType Parser::Factor(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  Set constant = First::Constant();
  Set varacc = First::VariableAccess();
 mType localType;
  //
  // Do we need this still? It seems like it was supposed to be used in those MUNIONS
    //Set *temp = new Set(")");
  //

    
  if (First::FactorName().isMember(lookAheadToken.getLexeme())) 
  {
    localType = FactorName(sts);
  }
  else if (First::Constant().isMember(lookAheadToken.getLexeme()))
    localType = Constant(sts);
  else if (lookAheadToken.getLexeme() == "(")
  {
    match("(",sts.munion(First::Expression()).munion(Set(")"))); 
    localType = Expression(sts.munion(Set(")"))); 
    match(")",sts);
    
  }
  //or
  else if ( lookAheadToken.getLexeme() == "~")
  {
    match("~",sts.munion(First::Factor())); 
    localType = Factor(sts);
  }
  
  
  syntaxCheck(sts);
  return localType;
}


mType Parser::FactorName(Set sts) 
{
  debug(__func__, sts, lookAheadToken);
  mType localType;
  if (lookAheadToken.getLexeme() == "name") 
  {
    localType = VariableName( sts.munion(First::Constant()).munion(First::VariableAccess()));
    if (First::IndexedSelector().isMember(lookAheadToken.getLexeme()))
    {
	IndexedSelector(sts);
    }	
  } 
    
  else if (First::Constant().isMember(lookAheadToken.getLexeme())) 
  {
    if(First::Numeral().isMember(lookAheadToken.getLexeme()))
    {
      localType = Numeral(sts);
    }
    //or
    else if (First::BooleanSymbol().isMember(lookAheadToken.getLexeme()))
    {
      localType = BooleanSymbol(sts);
    }
        
  }
  
    
  syntaxCheck(sts);
  
  return localType;
  
    
}

/////////////////////////////////////////////////////////////////////////////
mType Parser::VariableAccess(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  mType localType;	
  Set indsel = First::IndexedSelector();
  localType = VariableName(sts.munion(First::IndexedSelector()));
  //one or zero of thefollowing
  
  if(indsel.isMember(lookAheadToken.getLexeme()))
  {
    IndexedSelector(sts);
  }
  
  syntaxCheck(sts);
  return localType;
}
/////////////////////////////////////////////////////////////////////////////
void Parser::IndexedSelector(Set sts)
{
	debug(__func__, sts, lookAheadToken);
	
	Set *temp = new Set("]");
  
  match("[", sts.munion(First::Expression()).munion(*temp)); 
  
  if (Expression(sts.munion(*temp)) != INTEGER)
  {
    cout << "Index selector evaluates to non integer type.  Please ensure only integers exists between [ and ]." << endl;
    numberOfScopeTypeErrors++;
    cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
  }
  match("]", sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
mType Parser::Constant(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  Set num = First::Numeral();
  Set bol = First::BooleanSymbol();
  Set con = First::ConstantName();
  
  mType localType;
  
  if(num.isMember(lookAheadToken.getLexeme()))
  {
    localType = Numeral(sts);//////////////////////////////>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>.
  }
  //or
  else if (bol.isMember(lookAheadToken.getLexeme()))
  {
    localType = BooleanSymbol(sts);
  }
  //or
  else if (con.isMember(lookAheadToken.getLexeme()))
  {
    localType = ConstantName(sts);
  }
  
  syntaxCheck(sts);
  
  return localType;
}
/////////////////////////////////////////////////////////////////////////////
mType Parser::Numeral(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  match("num", sts);
  
  syntaxCheck(sts);

  return INTEGER;
}
/////////////////////////////////////////////////////////////////////////////
mType Parser::BooleanSymbol(Set sts)
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
  
  return BOOLEAN;
}
/////////////////////////////////////////////////////////////////////////////
mType Parser::ConstantName(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  //token must be a reserved word TODO - might not be a reserved word.
  mType type = VariableName(sts);
  
  syntaxCheck(sts);
  return type;
}
/////////////////////////////////////////////////////////////////////////////
mType Parser::VariableName(Set sts)
{
  debug(__func__, sts, lookAheadToken);
	  //token must be a user defined word
  mType type;
  bool error = false;
  TableEntry entry;
  if (blockTypeStack.top() == DEFINITIONPART)
    type = UNIVERSAL;
  else if (blockTypeStack.top() == STATEMENTPART) {
    
   entry = blocktable->find(lookAheadToken.getValue(), error);
   
   if (!error) 
   {
     type = entry.otype;
   }
   else
   {
     type = UNIVERSAL;
     cout << "Undeclared variable. Maybe a typo? Check variable declaration section." << endl;
     numberOfScopeTypeErrors++;
     cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
   }
   
  }
    
  match("name", sts);
  
  syntaxCheck(sts);
  //jp debug info - leave - mar 31 2012
  //cout<<"**************************************VariableNameReturnType="<<blocktable->convertType(type)<<endl;
  return type;
}
///////////////////////////g//////////////////////////////////////////////////
mType Parser::ProcedureName(Set sts)
{
	debug(__func__, sts, lookAheadToken);
  mType type = VariableName(sts);
  syntaxCheck(sts);
  return type;
}


void Parser::resetPrevMatches() {
	prevMatch[0] = "";
	prevMatch[1] = "";
	prevMatch[2] = "";
	prevMatch[3] = "";
}
