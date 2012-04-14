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
  //Attach the symbol table and administrative object.
  admin = &adminObject;
  table = &symTable;
  
  //DEBUGGING flag. Set to TRUE in order to see function names and Parse tree status.
  debugflag = false;
  
  //We can't START with errors, after all.
  errorCount = 0;
  numberOfScopeTypeErrors = 0;
  
  // Attach a new blocktable to our Parser. This should be unique.
  blocktable = new BlockTable(*table);
  

  
  //Begin the Parsing/scanning.
  lookAheadToken = nextToken();
  
  // start negative so labelcount++ will give 0
  labelCount = 0;
}

/** Base destructor. */
Parser::~Parser()
{
}

int Parser::NewLabel() 
{
  labelCount += 1;
  return labelCount;
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
  
  //begin the parsing using the Program production, with the above Stop Set.
  this->Program(*temp);
  
  //the parser has ended by now.
  cout << endl;
  cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
  cout << "Parsing completed." << endl;
  
  if(errorCount > 0)
    cout << "Parsing Errors Found: " << errorCount << endl;
  else
    cout << "No Parsing errors found. " << endl;;
  
  cout << "Scope and Type check Completed." << endl;
  if(numberOfScopeTypeErrors > 0)
    cout << "Scope and Type Check Errors Found: " << numberOfScopeTypeErrors << endl;
  else
    cout << "No Scope or Type check errors found. "<< endl;
  
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
 * Matches the input lexeme to that of the next token.
 * Otherwise, we generate and error.
 * Finally, we check for a valid syntax set.
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
/**
 * @brief Cleans up after an error has been detected while matching or checking syntax.
 * 14240815

 * Increases the error count, outputs the relevent error information, then loops
 * until it encounters a valid set of next parseable characters.
 *
 */
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
/**
 * @brief Checks if the next Token (its lexeme) is of a valid set of characters.
 *
 * Checks if the the lexeme of the nlookahead/next token is in the set of
 * valid characters, and generates a Syntax Error if they are not.
 * That is, if the next character isn't valid, then there's a syntax error.
 * 
 */
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
 * In order to guarantee Semantic correctness and correct Scope validity, some
 * productions need to return their mType or the mType of their proceeding operations
 * which is then compared against the set of proper possible mTypes for the given
 * production rules to guarantee that it's valid.
 *
 * i.e. we cannot have Integer X := 5 in the DefinitionPart.
 * For scope checking, we check the current and preceeding blocks to see if the 
 * currently examined table entry/tokenized object is semantically valid and not
 * being redeclared or improperly used.
 * The order and syntax of such operations varies with each specific production,
 * and thus does not fit into the 8-point checklist above.
 *
 * @see parser.h
 */


/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief The root production of A PL program, the Program production.
 * 
 * The PL program should start here, and ends with the ./$ sign set.
 * From here, the first, base Block is created and called.
 */
void Parser::Program(Set sts)
{
  
  debug(__func__, sts, lookAheadToken);
  Set* temp = new Set(".");
  
  int startLabel, varLabel;
  //there is a counter that returns next label (an integer) to each call to NewLabel function
  
  //this is to record the address of the first instruction of the program
  startLabel = NewLabel();
  //this is storage needed for the variables
  varLabel = NewLabel();
  
  //output the instruction prog which sets up the activation record for program and executes first instruction
  //at startLabel (to be computed by assembler)

  admin->emit3("PROG", varLabel, startLabel);
  Block(startLabel, varLabel, sts.munion(*temp)); 
  //putput end of program instruction endprog

  admin->emit1("ENDPROG");
  match(".", sts);
  
  syntaxCheck(sts);
  
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief A Block grammar component, which equates to a specific program Scope
 * 
 * Each Block contains a DefinitionPart and StatementPart, and indicates that a
 * new program Scope has been entered. Thus, the internal parts have access to
 * the previous Block entries (scope data) but the StatementPart AFTER this block
 * (if any) will not have access to the information created in this Block production
 * (we use endBlock to ensure this.)
 * 
 */

void Parser::Block(int beginLabel, int varLabel, Set sts)
{
  debug(__func__, sts, lookAheadToken);
  Set* temp = new Set("end");
  
  
  match("begin",sts.munion(First::DefinitionPart()).munion(First::StatementPart()).munion(*temp));
  
  /** A new scope has started. */
  blocktable->newBlock();
  
  //total variable storage requirement
  int varLength = 0;
  //retriving varLength from the DefinitionPart function
  varLength = DefinitionPart(sts.munion(First::StatementPart()).munion(*temp)); 
  //defin the labels used in PROC and PROG operations.
  //output assembler instruction DEFARG to enter labelTable[varLabel] = varLength in pass 1 so that
  // varLength replaces varLabel in the final code output in pass 2 of the assembler

  admin->emit3("DEFRAG", varLabel, varLength);
  //we are about to begin the first executable instruction - output assembler instruction DEFADDR to enter
  //labelTable[startLabel] = address   for next instruction

  admin->emit2("DEFADDR", beginLabel);
  
  StatementPart (sts.munion(*temp)); 
  //blocktable->printAllBlocks();

  /** The previous scope (block) has ended. */
  match("end", sts);
  blocktable->endBlock();
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief The latter half of a Block, the DefinitionPart.
 * 
 * The DefinitionPart is where PL constructs are first declared, though only
 * given distinct values in the cases of Proc names, Const values, and array sizes.
 * 
 * The block type is recorded to ensure that Expressions and guards evaluate properly.
 * 
 */

int Parser::DefinitionPart(Set sts)
{
  /** Used to guarantee that the proceeding productions have the right semantic types when checked. */
  blockTypeStack.push(DEFINITIONPART);
  
  debug(__func__, sts, lookAheadToken);
  Set* temp = new Set(";");
  Set first = First::Definition();
  
  //total storage needed for variables declared within this particular blcok
  int varlength = 0;
  //variable start is offset for the dynamixc link, static link, and return address within a stack frame
  int nextvarstart = 3;
  
  //optional part
  //can be one or more, or nothing here
  //have to check if the lookahead is in the first of definition, and if not, 
  //then check if it is in the first of statement part from block
  /** Loops through until we find a Token that isn't a valid Definition/is the end of the DefinitionPart */
  while(first.isMember(lookAheadToken.getLexeme()))
  {
    varlength += Definition(nextvarstart, sts.munion(*temp)); 
    match(";", sts.munion(First::Definition()));//aded in first of definition so the while loop can keep going.
  }
  /** Since we are no longer in the DefinitionPart, we pop it off the Type stack. */
  blockTypeStack.pop();
  syntaxCheck(sts);
  
  return varlength;
  
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief A component Definition, which is of one of three types.
 * 
 * No special type checking or scope rules occurs here, but rather, in one of
 * the proceeding functions.
 *
 * A Definition can be one of these three types: Constant, Variable or Procedure;
 * an array is of type Variable, of course.
 *
 * @see DefinitionPart()
 */
int Parser::Definition(int& varStart, Set sts)
{
  debug(__func__, sts, lookAheadToken);
  
  /** We simply check which kind of Definition the next token belongs to. */
  if(First::ConstantDefinition().isMember(lookAheadToken.getLexeme()))
  {
    ConstantDefinition(sts);
    return 0;
  }
  else if(First::VariableDefinition().isMember(lookAheadToken.getLexeme()))
  {
    return VariableDefinition(varStart, sts);
  }
  else if(First::ProcedureDefinition().isMember(lookAheadToken.getLexeme()))
  {
    ProcedureDefinition(sts);
    return 0;
  }
  
  syntaxCheck(sts);
  
  return 0;
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Productions for the Definition type of Constant are tested.
 * 
 * Consists of "const" ConstantName "=" Constant ->
 * 
 * Thus, we check to make sure each component is value, and check the Constant 
 * and ConstantName productions.
 *
 * Here, is where we check to see that the Constant information does not exist
 * in our scope already, and generate an error if it does.
 *
 */
void Parser::ConstantDefinition(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  //The default StopSet for a Constant definition, to be union'd with our input stopset from definition.
  Set* temp = new Set("=");
  
  int tokenID = 0;
  int constVal = 0;
  
  //The type of tableEntry of the constant.
  mType type = UNIVERSAL;
  
  //Verify that out current set matches that of what is allowed for a Const.
  match("const", sts.munion(First::ConstantName()).munion(*temp).munion(First::Constant())); 
  
  //Grab the token Value from the lookaheadtoken.
  tokenID = lookAheadToken.getValue();
  
  ConstantName(sts.munion(*temp).munion(First::Constant())); 
  match("=", sts.munion(First::Constant())); 
  
  //
  if (lookAheadToken.getLexeme() == "num") 
  {
    constVal = lookAheadToken.getValue();
    type = INTEGER;
  }
  else if (lookAheadToken.getLexeme() == "true" || lookAheadToken.getLexeme() == "false") 
  {
    type = BOOLEAN;
    if (lookAheadToken.getLexeme() == "true")
      constVal = 1;
    else 
    {
      constVal = 0;
    }
  } 
  else if (lookAheadToken.getLexeme() == "name") 
  {
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
/**
 * @brief Processes a Variable according to Definition
 * 
 * Grabs the type of an input symbol and passes it to the VariableDefinitionPart.
 * This allows us to properly process Arrays or standard VARIABLEs and attach them
 * to the blocktable and block and current scope as required, while checking their components
 * (such as the array index specifier) accordingly.
 * 
 */
int Parser::VariableDefinition(int& varStart, Set sts)
{
  debug(__func__, sts, lookAheadToken);
  mType type = TypeSymbol(sts.munion(First::VariableDefinitionPart())); 
  int tempVal = VariableDefinitionPart(sts, varStart, type);
  
  syntaxCheck(sts);
  
  return tempVal;
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Defines the definition of a VariableList, or an array
 * 
 * Array as defined; "array" VariableList "[" Constant "]"
 * or it's just VariableList
 *
 * Guarantees that the valid prodution is chosed by examining the type
 * of the variable and the corresponding sets resultant from the lookahead.
 *
 * Checks the validity of Array components if necessary, and creates the
 * Array to specification within the BlockTable.
 * 
 */
int Parser::VariableDefinitionPart(Set sts, int &varStart, mType type)
{
  debug(__func__, sts, lookAheadToken);
  Set* temp = new Set("[");
  Set* temp2 = new Set("]");
  vector<int> arrayIDs;
  
  if(First::VariableList().isMember(lookAheadToken.getLexeme()))
  {
   arrayIDs = VariableList(sts, type, VARIABLE);
   return arrayIDs.size();
  }
  else 
  {
    bool error = false;
    TableEntry entry;
    int constid;
    
    
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
      cout << "Array Size variable is of non-Integer type.  Trying to create array size with type of " << blocktable->convertType(entry.otype) << endl;
      numberOfScopeTypeErrors++;
      cout << "Found at line: " << admin->getLineNumber() << ", Column: "<< admin->getColumnNumber() << endl;
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
      cout << "Found at line: "<< admin->getLineNumber() << ", Column: "<< admin->getColumnNumber() << endl; 
    }
  syntaxCheck(sts);
  return arrayIDs.size() * entry.value; 
  }
  
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Processes a TypeSymbol, of Boolean or Integer design
 * 
 * Returns a an mType from a typesymbol,
 * Checks the lookahead and returns the valid type (int or bool)
 * or returns a UNIVERSAL (error) type.
 */
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
/**
 * @brief Processes a variable list and guarantees a proper set of variable types.
 * 
 * Checks for variables as it creates their block and table entries,
 * and returns a vector of their ids.
 * 
 */
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
/**
 * @brief Defines a Procedure by name
 * 
 * Defines the Procedure and processes its name accordingly.
 * Adds it to the current/given block.
 * 
 */
void Parser::ProcedureDefinition(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  
  match("proc",sts.munion(First::ProcedureName()).munion(First::Block())); 
  
  int procLabel = NewLabel();
  
  blocktable->define(lookAheadToken.getValue(), PROCEDURE, UNIVERSAL);
  ProcedureName(sts.munion(First::Block())); 
  
  
  int startLabel = NewLabel();
  int varLabel = NewLabel();
  

  admin->emit2("DEFADDR", procLabel);
  admin->emit3("PROC", varLabel, startLabel);
  
  
  Block(startLabel, varLabel, sts);

  admin->emit1("ENDPROC");
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Processes the StatementPart of a Block
 * 
 * Adds StatementPart to the blocktype stack and calls the statementpart productions (statement).
 * Pops accordingly after successful parsing.
 * 
 */
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
  }
  blockTypeStack.pop();
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Processes a Statement, which can be one of many.
 * 
 * Processes possible statements, only one input to many possibilities.
 * 
 */
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
  vector<mType> countMe = VariableAccessList(sts);
  
  admin->emit2("READ", countMe.size());
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Processes a list of VariableAccess productions
 * 
 * Returns a vector of the corresponding types of variableaccess vars.
 * 
 */
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
  
  vector <mType> callMeMaybe = ExpressionList(sts);
  
  admin->emit2("WRITE", callMeMaybe.size());
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Evaluates a list of Expressions for use by Statements.
 * 
 * Grabs all of the types of the expression(s) and stores them in a vector,
 * which is later returned.
 * 
 */
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
  
    return types;
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Processes an AssignmentStatement grammar component, a subpart of Statement
 * 
 * Uses two vectors of mTypes to match up to Variable and Expression lists, and guarantee
 * their types match correspondingly, or outputs a type mismatch error therein.
 * 
 */
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
    
    bool allOk = true;
    int indexSelect;
    
    for(int i = 0; i < (int) val.size(); i++)
    {
      
      if(val.at(i) == CBOOLEAN)
      {
	//error - trying to redefine constant variable
	cout<<"Error - Attempting to redefine a constant Boolean variable within a assignment statement.  Constant located at place (from 0) "<<i<<endl;
	numberOfScopeTypeErrors++;
	cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
	
      }
      else if (val.at(i) == CINTEGER)
      {
	//error - trying to redefine constant variable
	cout<<"Error - Attempting to redefine a constant Integer variable within a assignment statement.  Constant located at place (from 0) "<<i<<endl;
	numberOfScopeTypeErrors++;
	cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
	
      }     
      else if(val.at(i) == INTEGER && el.at(i) == CINTEGER)
      {
	//all is well
      }
      else if (val.at(i) == BOOLEAN && el.at(i) == CBOOLEAN)
      {
	//all is well
      }
      else if(val.at(i) != el.at(i))
      {
	allOk = false;
	indexSelect = i;
      }
    }
    if (!allOk)
    {
      cout<<"Assignment type mismatch at place (from 0) " <<indexSelect<<". Trying to assign " << blocktable->convertType(val.at(indexSelect))<<" to "<< blocktable->convertType(el.at(indexSelect)) <<endl;
      numberOfScopeTypeErrors++;
      cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
    }
    else
    {
      admin->emit2("ASSIGN", val.size());
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
/**
 * @brief Processes a subtype of Statement, a Proc
 * 
 * Checks to see if the Procedure exists in a previous Scope/Block.
 * If not, then the call was invalid/procname does not match or exist,
 * otherwise it just returns the command properly.
 * 
 */
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
  
  //make the call with the info found in the table entry
  //what is entry.startLabel? where in the world is carmen sandiego?
  admin->emit3("CALL", blocktable->currentLevel() - entry.level, entry.startLabel);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Verifies the If guard process
 * 
 * Checks for proper If components, while calling for a possible GuardedCommandList
 * 
 */
void Parser::IfStatement(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  Set *temp = new Set("fi");
  
  match("if",sts.munion(First::GuardedCommandList()).munion(*temp)); 
  
  int startLabel = NewLabel();
  int doneLabel = NewLabel();
  
  GuardedCommandList(startLabel, doneLabel, sts.munion(*temp)); 

  admin->emit2("DEFADDR", startLabel);
  
  admin->emit2("FI", admin->getLineNumber());
  
  admin->emit2("DEFADDR", doneLabel);
  
  match("fi",sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief processes the Do guarded command.
 * 
 * Part of guaranteeing the Guards are correct.
 * Matches the relevent guards checks that the subsequent guarded command list is valid.
 * 
 */
void Parser::DoStatement(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  Set *temp = new Set("od");
  
  match("do",sts.munion(First::GuardedCommandList()).munion(*temp)); 
  
  int startLabel = NewLabel();
  int loopLabel = NewLabel();
  

  admin->emit2("DEFADDR", loopLabel);
  GuardedCommandList(startLabel, loopLabel, sts.munion(*temp)); 

  admin->emit2("DEFADDR", startLabel);
  match("od",sts);
  
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Matches the valid guarded commands along with the specifiers []
 * 
 * Verifies that the GuardedCommands are valid, and each subsequent guarded 
 * command in the list is valid as well.
 * 
 */
void Parser::GuardedCommandList(int& startLabel, int GoTo,Set sts)
{
  debug(__func__, sts, lookAheadToken);
  Set *temp = new Set("[]");
  
  GuardedCommand(startLabel, GoTo, sts.munion(*temp).munion(First::GuardedCommand()));
  //optional
  while(lookAheadToken.getLexeme() == "[]")
  {
    match("[]",sts.munion(First::GuardedCommand())); 
    GuardedCommand(startLabel, GoTo, sts.munion(*temp));
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Matches the guards for a GuardedCommand via Expression type check
 * 
 * Brief...then evaluates the StatementPart of a Guarded command (do ..., if ...)
 * If the expression type is not a Boolean, then it throws the proper error.
 * Does not chain up.
 */
void Parser::GuardedCommand(int& thisLabel, int GoTo, Set sts)
{
  debug(__func__, sts, lookAheadToken);
  Set *temp = new Set("->");

  admin->emit2("DEFADDR", thisLabel);
  mType localType = Expression(sts.munion(*temp).munion(First::StatementPart()));
  
  if (localType != BOOLEAN && localType != CBOOLEAN)
  {
    //Need a better print out here - JP Mar31 2012
    cout << "Compound guarded command statement (entry conditions for while, do, for, if) did not evaluate to boolean." << endl;
    numberOfScopeTypeErrors++;
    cout << "Found at line: "<<admin->getLineNumber()<<", Column: "<<admin->getColumnNumber()<<endl;
  }
  thisLabel = NewLabel();

  admin->emit2("ARROW", thisLabel);
  match("->",sts.munion(First::StatementPart())); 
  StatementPart(sts);

  admin->emit2("BAR", GoTo);
  
  syntaxCheck(sts);
  
  
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Checks the PrimaryExpression evals to determine Eval's type(s)
 * Chains up to ExpressionList
 * Returns a type resulting from the corresponding PrimaryExpression evaluations
 * and uses a vector as (similar to SimpleExpression and Term) there is the possibility
 * of multiple types needing comparison;
 * This is because of the following grammar: Expression -> PrimaryExpression { PrimaryOperator PrimaryExpression }
 * 
 */
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
    if(localTypes.at(0) == CINTEGER)
      return INTEGER;
    else if (localTypes.at(0) == CBOOLEAN)
      return BOOLEAN;
    else
      return localTypes.at(0);
  }
  
  for(int i=0; i < (int)localTypes.size(); i++) 
  {
    if (localTypes.at(i) != BOOLEAN && localTypes.at(i) != CBOOLEAN)
    {  
      return UNIVERSAL;
    }
  }
  return BOOLEAN;
}
/////////////////////////////////////////////////////////////////////////////
void Parser::PrimaryOperator(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  if(lookAheadToken.getLexeme() == "&")
  {
    match("&",sts);
    admin->emit1("AND");
  }
  //or
  else if (lookAheadToken.getLexeme() == "|")
  {
    match("|",sts);
    admin->emit1("OR");
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief A Primary Expression is a SimpleExpression with an optional operator with a simple expression.
 *
 * Checks to make sure that the proper evaluations occur for a SimpleExpression;
 * One must evaluate to a boolean, but with multiples, it could be comparative, or equivalency.
 * Thus we check to see what type of Factors are resultant from the simpleexpression terms.
 * 
 */
mType Parser::PrimaryExpression(Set sts)
{
  mType type1, type2;
  debug(__func__, sts, lookAheadToken);
  bool type2found = false;
  
  /** We've come to a functional specifier, so we'll either return a boolop if the factor is valid, or an error type. */
  if (lookAheadToken.getLexeme() == "~" || lookAheadToken.getLexeme() == "(") 
  {
    type1 = Factor(sts);
    
    if (type1 == BOOLEAN || type1 == CBOOLEAN)
      return type1;
    else
      return UNIVERSAL;
  } 
  /** It's a SimpleExpression akin to a term-kind of some sort, so we'll check the corresponding production types. */
  else 
  {
    
    type1 = SimpleExpression(sts.munion(First::RelationalOperator()).munion(First::SimpleExpression()));
    //1 or zero of the follwing
    /** a relational operator follows the first SimpleExpression, thus we know that we will have to have int comparisons 
     * rather than returning a boolean */
    if(First::RelationalOperator().isMember(lookAheadToken.getLexeme()))
    {
      type2found = true;
      
	  string currentLexeme = lookAheadToken.getLexeme();
	  RelationalOperator(sts.munion(First::SimpleExpression())); 
      type2 = SimpleExpression(sts);
	  
	if (currentLexeme == ">")
		admin->emit1("GREATER");
	else if (currentLexeme == "=")
		admin->emit1("EQUAL");
	else if (currentLexeme == "<")
		admin->emit1("LESS");
      
    }
    
    syntaxCheck(sts);
    
    /** Clarify that, since there are two SimpleExpressions, that their types are comparable in terms of boolean ops */
    if(type2found)
    {
      if ( (type1 == INTEGER || type1 == CINTEGER) && (type2 == INTEGER || type2 == CINTEGER) )
      {
	return BOOLEAN;
      }
      else
      {
	return UNIVERSAL;
      }
    }
    
    if(!type2found)
    {
      if(type1 == INTEGER || type1 == CINTEGER) 
      {
	return INTEGER;
      }
      else if (type1 == BOOLEAN || type1 == CBOOLEAN)
      {
	return BOOLEAN;
      }
      else
      {
	return UNIVERSAL;
      }
    } 
  }
  return UNIVERSAL;
  
}
/////////////////////////////////////////////////////////////////////////////
void Parser::RelationalOperator(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  if(lookAheadToken.getLexeme() == "<")
  {
    match("<",sts);
   /// admin->emit1("LESS");
  }
  //or
  else if(lookAheadToken.getLexeme() == "=")
  {
    match("=",sts);
   // admin->emit1("EQUAL");
  }
  //or
  else if(lookAheadToken.getLexeme() == ">")
  {
    match(">",sts);
   // admin->emit1("GREATER");
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief A SimpleExpression must evaluate to an Integer type and defines a simple expression.
 * 
 * A vector of mTypes is used because a SimpleExpression is like a Term; One or Many of its subcomponents.
 * A SimpleExpression is composed of Multiple terms, a possible sub operator, and multiple Adding operators
 * correspondingly. Thus, we verify that the terms and operator types evaluate properly, and return the
 * corresponding result.
 * 
 */
mType Parser::SimpleExpression(Set sts)
{
  /** Vector to track the subcomponent types. */
  vector<mType> localtypes;
  debug(__func__, sts, lookAheadToken);
  //1 or zero of the following
  if(lookAheadToken.getLexeme() == "-")
  {
    admin->emit1("MINUS");
    match("-",sts.munion(First::Term()).munion(First::AddingOperator()));
  }
  
  
  //required
  localtypes.push_back(Term(sts.munion(First::AddingOperator()).munion(First::Term())));
  
	string currentlexeme = lookAheadToken.getLexeme();

  //Optional
  while(First::AddingOperator().isMember(lookAheadToken.getLexeme()))
  {
    Set *temp = new Set("+");
    Set *temp2 = new Set("-");
    AddingOperator(sts.munion(First::Term())); 
    
	

    localtypes.push_back(Term(sts.munion(*temp).munion(*temp2)));

	if (currentlexeme == "+")
			admin->emit1("ADD");
		else if (currentlexeme == "-")
			admin->emit1("SUBTRACT");

	currentlexeme = lookAheadToken.getLexeme();
  }
  
  
  syntaxCheck(sts);
  
  //this is to check when we only see one factor here.  then we just return that one factor's type,
  //instead of checking if they are all INTEGER, casue if there is only one, it doesnt have to be just INTEGER
  if((int)localtypes.size() == 1)
  {
    if(localtypes.at(0) == CINTEGER)
      return INTEGER;
    else if(localtypes.at(0) == CBOOLEAN)
      return BOOLEAN;
    else
      return localtypes.at(0);
  }
  
  for(int i=0; i < (int)localtypes.size(); i++) 
  {
    if ((localtypes.at(i) != INTEGER) && (localtypes.at(i) != CINTEGER))
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
    //admin->emit1("ADD");
  }
  //or
  else if(lookAheadToken.getLexeme()=="-")
  {
    match("-",sts);
    //admin->emit1("SUBTRACT");
  }
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief A Term is part of a SimpleExpression, and thus Primary Expression.
 * 
 * A term can actually be a logical series of Terms, as they occur over multiple
 * expressions. Thus, we need to keep track of them in an mType vector.
 *
 * Eventually, this chains back up to SimpleExpression, then PrimaryExpression, then Expression.
 * The Term is a core/central production language grammar component.
 *
 * Term = Factor { MultiplyingOperator Factor }.
 * 
 */
mType Parser::Term(Set sts)
{
  /** A vector of types to account for a series of terms. */
  vector<mType> localtypes;
  debug(__func__, sts, lookAheadToken);
  
  /** We grab the relevant Term mType from a Factor, the base of a Term production */
  localtypes.push_back( Factor(sts.munion(First::MultiplyingOperator()).munion(First::Factor())));
  //redundant check for if it is in factor?
  //casue below we have a while statement that chceks only for if it is in multilpfying opertor.
  //so program could come back here thinking first of factor is correct, but then we dont use it here,
  //only use first of multpiying operator......? TODO
  
  /** Used to grab the proper Multiplicative symbol for ASSEM output. */
  string currentLexeme = lookAheadToken.getLexeme();
  
  //optional
  /** If we have a qualifying MultiplyingOperator, then we keep adding the Factor terms to the
   * mType vector until we are no longer processing Term components. @see Term-Grammar Production. */
  while(First::MultiplyingOperator().isMember(lookAheadToken.getLexeme()))
  {
    MultiplyingOperator(sts.munion(First::Factor()));  
    
    Set *temp = new Set("*");
    Set *temp2 = new Set("/");
    Set *temp3 = new Set("\\");
    localtypes.push_back( Factor(sts.munion(*temp).munion(*temp2).munion(*temp3)));
    
    /** We output after the pushback with FACTOR in order to get the postfix form. */
    if(currentLexeme == "*")
      admin->emit1("MULTIPLY");
    else if(currentLexeme == "/")
      admin->emit1("DIVIDE");
    else if(currentLexeme == "\\")
     admin-> emit1("MODULO");
    
    currentLexeme = lookAheadToken.getLexeme();
  }
  
  syntaxCheck(sts);
  
  //this is to check when we only see one factor here.  then we just return that one factor's type,
  //instead of checking if they are all INTEGER, casue if there is only one, it doesnt have to be just INTEGER
  if((int)localtypes.size() == 1)
  {
    if(localtypes.at(0) == CINTEGER)
      return INTEGER;
    else if(localtypes.at(0) == CBOOLEAN)
      return BOOLEAN;
    else
      return localtypes.at(0);
  }
  
  for(int i=0; i < (int)localtypes.size(); i++) 
  {
    if ((localtypes.at(i) != INTEGER) && (localtypes.at(i) != CINTEGER))
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
/**
 * @brief Processes the Factor's possible productions and deterimines the correct types.
 * 
 * Calls FactorName instead of VariableName, to remove an ambiguity.
 * Determines the type of Factor and returns it, or an error. 
 *
 */
mType Parser::Factor(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  Set constant = First::Constant();
  Set varacc = First::VariableAccess();
  mType localType;
  
  
  
  if (First::FactorName().isMember(lookAheadToken.getLexeme())) 
  {
    TableEntry entry;
    bool error = false;
    entry = blocktable->find(lookAheadToken.getValue(), error);
    if (entry.okind == VARIABLE)
    { 
      /*
      cerr << "\nPrinting Value Info\n" << "Displacement " <<  entry.displacement << " id " << entry.id << " level " <<
	  entry.level << " size " << entry.size << "okind " << blocktable->convertKind(entry.okind) << " otype " << blocktable->convertType(entry.otype) << endl;
      */
      bool error = false;
      TableEntry entry;
      entry = blocktable->find(lookAheadToken.getValue(), error);
  
  
  /**
   * *
   * *
   * *
   * *
   * *
   * 
   * FIX TO 1 BY REMOVING THE -1 ALSO IN FACTOR NAME.
   * ED is currently starting at 1, but we think el should start at 1 but it starts at 0.
   * We would have to change - , +, AND, OR, >, < possibly to the post-fix form. Also EQUALS
   * *
   */
      admin->emit3("VARIABLE", entry.level - 1, entry.displacement);
      admin->emit1("VALUE");
    }
    
    localType = FactorName(sts);
  }
  else if (First::Constant().isMember(lookAheadToken.getLexeme()))
  {
    admin->emit2("CONSTANT", lookAheadToken.getValue()); 
    localType = Constant(sts);
  }
  else if (lookAheadToken.getLexeme() == "(")
  {
    match("(",sts.munion(First::Expression()).munion(Set(")"))); 
    localType = Expression(sts.munion(Set(")"))); 
    match(")",sts);
    //no emit needed here
    
  }
  //or
  else if ( lookAheadToken.getLexeme() == "~")
  {
    match("~",sts.munion(First::Factor())); 
    admin->emit1("NOT");
    localType = Factor(sts);
  }
  
  
  syntaxCheck(sts);
  return localType;
}

/**
 * @brief A FactorName is a VariableAccess for resolving FACTOR productions.
 * 
 * Returns the given type of the factor (numeral, boolean or universal).
 * Alternatively, if the next token is a VariableName, it grabs the type that
 * said Variable corresponds to.
 * 
 */
mType Parser::FactorName(Set sts) 
{
  debug(__func__, sts, lookAheadToken);
  mType localType;
  
  /** If it's a name, grab its type, and process the index if it's an array */
  if (lookAheadToken.getLexeme() == "name") 
  {
    localType = VariableName( sts.munion(First::Constant()).munion(First::VariableAccess()));
    if (First::IndexedSelector().isMember(lookAheadToken.getLexeme()))
    {
      IndexedSelector(sts);
    }	
  } 
  /** Otherwise, it's either a Numeral or a Boolean symbol type. Process them. */
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
/**
 * @brief The production for grabbing a Variable from a VariableAccessList
 * 
 * We check to make sure that the index selector is valid, then chain up the
 * type from the given VariableName.
 * 
 */
mType Parser::VariableAccess(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  mType localType;	
  
  /** Set the index selector stop set. */
  Set indsel = First::IndexedSelector();

  //grabbing the next token and looking it up in the blocktable, finding the level and displacement and
  //emit the variable signal and information
  bool error = false;
  TableEntry entry;
  entry = blocktable->find(lookAheadToken.getValue(), error);
  admin->emit3("VARIABLE", entry.level - 1, entry.displacement);
  
  /** Grab the proper type from the variable name. */
  localType = VariableName(sts.munion(First::IndexedSelector()));


  

  
  /** Process the indexedSelector data, if valid. */
  //one or zero of thefollowing
  if(indsel.isMember(lookAheadToken.getLexeme()))
  {
    admin->emit3("INDEX", entry.size, admin->getLineNumber());
    IndexedSelector(sts);
  }
  
  syntaxCheck(sts);
  return localType;
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief The index selector for a List, such as a VariableAccessList
 * 
 * We check to ensure that the list accessor has the specified symbols ([_])
 * An indexed selector is composed of [ EXPRESSION ]. 
 * Chains up to VariableAccessList through VariableAccess.
 */
void Parser::IndexedSelector(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  
  Set *temp = new Set("]");
  
  //check for left of index selector
  match("[", sts.munion(First::Expression()).munion(*temp)); 
  
  /** Determine the type of expression for this selector. */
  mType localType = Expression(sts.munion(*temp));
  
  
  /** the Indexed Selector is not an integer or constant integer, and thus is not valid! */
  if (localType != INTEGER && localType != CINTEGER)
  {
    cout << "Index selector evaluates to non integer type.  Please ensure only integers exists between [ and ]." << endl;
    cout << "Current index selector is of type "<< blocktable->convertType(localType) << "." << endl;
    numberOfScopeTypeErrors++;
    cout << "Found at line: " << admin->getLineNumber() << ", Column: "<< admin->getColumnNumber() << endl;
  }
  //check for right of index selector
  match("]", sts);
  
  syntaxCheck(sts);
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Checks validity of a Constant in an input program string.
 * 
 * Either in a constantDefinition or an Array definition,
 * Can be a Numeral, BooleanSymbol or ConstantName
 *
 * A constant can be one of multiple types, and thus we check which one
 * while checking the validity and let that mtype information chain upwards.
 * 
 */
mType Parser::Constant(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  
  /** Create firstsets of the possibilities of a Constant. */
  Set num = First::Numeral();
  Set bol = First::BooleanSymbol();
  Set con = First::ConstantName();
  
  mType localType;
  /** Check for validity of the matching token, which is one of these...*/
  if(num.isMember(lookAheadToken.getLexeme()))
  {
    localType = Numeral(sts);
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
  /** then we return the given matching type. */
  return localType;
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Guarantees that a Numeral was examined.
 * 
 * Checks for the proper lookahead for a Numeral stop set.
 * 
 */
mType Parser::Numeral(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  
 
  match("num", sts);
  
  syntaxCheck(sts);
  
  /** We have evaluated an INTEGER literal. */
  return INTEGER;
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Checks the validity of a Boolean at this point in the program.
 * 
 * Ensures the next token is proper for a boolean of some sort,
 * then returns a BOOLEAN mType for our semantic analysis.
 *
 */
mType Parser::BooleanSymbol(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  
  /** Makes sure our proceeding tokens are correct for a boolean expression. */
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
  
  /** Ensures that the Parser knows that a Boolean was evaluated. */
  return BOOLEAN;
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief We return the type of a constant name.
 * 
 * We're in the definition part at this point, and thus defining the const in its name.
 * 
 */
mType Parser::ConstantName(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  //token must be a reserved word TODO - might not be a reserved word.
  mType type = VariableName(sts);
  
  syntaxCheck(sts);
  return type;
}
/////////////////////////////////////////////////////////////////////////////
/**
 * @brief Production for a Variable Name
 * 
 * We either define a new Const/Proc/Array variable type (Universal), or
 * we check for the variable's existence (as the only other access is in a statement).
 * If it does not exist, then we're using an undeclared name.
 * Otherwise, we determine and return the variable's type to the previous production.
 * 
 */
mType Parser::VariableName(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  //token must be a user defined word
  mType type;
  bool error = false;
  TableEntry entry;
  
  /**
   * We set the type of a VariableName by checking
   * the kind of variable and its corresponding entry's oType.
   * I.e. A constant Integer.
   * If we're in the definition part, it's of a Universal type.
   * Otherwise, we search for it...if it isn't found, then we're using
   * an undeclared variable.
   */
  if (blockTypeStack.top() == DEFINITIONPART)
  {
    type = UNIVERSAL;
  }
  else if (blockTypeStack.top() == STATEMENTPART) 
  {  
    entry = blocktable->find(lookAheadToken.getValue(), error);
    if (!error) 
    {
      type = entry.otype;
      if(entry.okind == CONSTANT && entry.otype == INTEGER)
      {
	type = CINTEGER;
      }
      if(entry.okind == CONSTANT && entry.otype == BOOLEAN)
      {
	type = CBOOLEAN;
      }
    }
    /** The variable was not found, and we're not declaring it for the first time, so it's an error. */
    else
    {
      type = UNIVERSAL;
      cout << "Undeclared variable. Maybe a typo? Check variable declaration section." << endl;
      numberOfScopeTypeErrors++;
      cout << "Found at line: "<< admin->getLineNumber() << ", Column: " << admin->getColumnNumber() << endl;
    }
    
  }
  
  match("name", sts);
  
  syntaxCheck(sts);
  //jp debug info - leave - mar 31 2012
  //cout<<"**************************************VariableNameReturnType="<<blocktable->convertType(type)<<endl;
  /** Return whatever type we have. Universal is the stock type when we encounter an error. */
  return type;
}
///////////////////////////g//////////////////////////////////////////////////
/**
 * @brief The name of the Procedure is checked.
 * 
 * A type evaluated from a VariableName is returned.
 * A procedure's name is a string and should correspond to a variable name.
 * 
 */
mType Parser::ProcedureName(Set sts)
{
  debug(__func__, sts, lookAheadToken);
  mType type = VariableName(sts);
  syntaxCheck(sts);
  return type;
}