/**
 * @brief The definition for the First set-collection object and functions.
 *
 * @file first.cpp
 * 
 * This class is a deceptively simple one.
 * The First object is really just a collection of functions
 * within an easily modifiable class structure that returns the
 * "First set" of a given Nonterminal.
 *
 * Which is to say, the "first" terminals of a given Nonterminal
 * (or, in our case, function) is returned.
 *
 * For each of the following functions, a base set or sets are created with
 * the appropriate set values corresponding to the First set for the given
 * nonterminal that each function represents (by name).
 * After this, sets are created using the mUnion of the given function set
 * with the following productions' first sets.
 *
 * Finally, the static functions return the given sets and are accessible 
 * without the calss being instantiated.
 *
 * @see set.h
 * @see parser.cpp
 * @author Jordan Peoples, Chad Klassen, Adam Shepley
 * @date January 9th to March 1st, 2011
 **/

#include "first.h"

//
// If desired, for this AND the parser, we could explicit comments for each of these.
//
/** Base constructor for the first class. */
First::First() 
{
  //Something	
  //nothing
  //the other.
	
}

/**
 *
 * The following functions have been chunked according to logical
 * groups, but I avoided re-ordering them at all in case there was 
 * a reason for the ordering - such as following closely to the book's
 * production rules or something like that.
 *
 *For each of the following functions, a base set or sets are created with
 * the appropriate set values corresponding to the First set for the given
 * nonterminal that each function represents (by name).
 * After this, sets are created using the mUnion of the given function set
 * with the following productions' first sets.
 *
 * Finally, the static functions return the given sets and are accessible 
 * without the class being instantiated.
 *
 * In other words, every function here is representative of a Function in the
 * Parser, and delivers their Follow sets upon call.
 *
 */

/** Returns the First set of Program, which is of Block. */
Set First::Program() 
{
  return Block();	
}

/** Returns the First set of Block, which is of "begin". */
Set First::Block() 
{
  return Set("begin");	
}

/** Returns the First set of DefinitionPart, which is of Definition */
Set First::DefinitionPart()
{
  Set set = Set("");
  Set set2 = set.munion(Definition());
  return set2;
}

/** Returns the FS of Definition, which consists of ConstantDefinition, VariableDefinition and ProcedureDefinition. */
Set First::Definition() 
{
  return ConstantDefinition().munion(VariableDefinition()).munion(ProcedureDefinition());
}

/** Returns the First set of StatementPart, which is of Statement. */
Set First::StatementPart()
{
  Set set = Set("");
  Set set2 = set.munion(Statement());  //TODO does this actually change set anymore? casue we made it so it didnt change the oroiginal set
  //so maybe this is not working right anymore, as sets original set is no changed, but rather a ducpliate variable is changed.
  //Should work now.
  return set2;
}

/** Returns the First set of Statement, which consists of that of EmptyStatement,
 * ReadStatement, WriteStatement, AssignmentStatement, ProcedureStatement,
 * IfStatement and DoStatement. 
 */
Set First::Statement()
{
  return EmptyStatement().munion(ReadStatement()).munion(WriteStatement()).munion(AssignmentStatement()).munion(ProcedureStatement())
  .munion(IfStatement()).munion(DoStatement());
}

/** Returns the First set of Constant, which is that of BooleanSymbol and ConstantName. */
Set First::Constant()
{
  return Numeral().munion(BooleanSymbol()).munion(ConstantName());
}

/** Returns the First set of ProcedureName, which is "name". */
Set First::ProcedureName()
{
  return Set("name");
}

/** Returns the First set of EmptyStatement, which is "skip" */
Set First::EmptyStatement()
{
  return Set("skip");
}

/** Returns the First set of ReadStatement, which is "read". */
Set First::ReadStatement()
{
  return Set("read");
}

/** Returns the First set of WriteStatement, which is "write". */
Set First::WriteStatement()
{
  return Set("write");
}

/** Returns the First set of AssignmentStatement, which is that of VariableAccessList. */
Set First::AssignmentStatement()
{
  return VariableAccessList();
}

/** Returns the First set of ProcedureStatement, which is "call". */
Set First::ProcedureStatement()
{
  return Set("call");
}

/** Returns the First set of IfStatement, which is "if". */
Set First::IfStatement()
{
  return Set("if");
}

/** Returns the First set of DoStatement, which is of "do". */
Set First::DoStatement()
{
  return Set("do");
}

/** Returns the First set of VariableAccessList, which is that of VariableAccess. */
Set First::VariableAccessList()
{
  return VariableAccess();
}

/** Returns the First set of VariableAccess, which is that of VariableName and IndexedSelector. */
Set First::VariableAccess()
{
  return VariableName().munion(IndexedSelector());
}

/** Returns the First set of ExpressionList, which is that of Expression. */
Set First::ExpressionList()
{
  return Expression();
}

/** Returns the First set of Expression, which is that of PrimaryExpression. */
Set First::Expression()
{
  return PrimaryExpression();
}

/** Returns the First set of GuardedCommandList, which is of GuardedCommand. */
Set First::GuardedCommandList()
{
  return GuardedCommand();
}

/** Returns the First set of GuardedCommand, which is of Expression. */
Set First::GuardedCommand()
{
  return Expression();
}

/** Returns the First set of PrimaryExpression, which is that of SimpleExpression. */
Set First::PrimaryExpression()
{
  return SimpleExpression();
}

/** Returns the First set of PrimaryOperator, which is "&" and "|". */
Set First::PrimaryOperator()
{
  Set set = Set("&");
  set.add("|");
  return set;
}

/** Returns the First set of SimpleExpression, which is "-" and that of Term. */
Set First::SimpleExpression()
{
  //TODO this may not be right.  setting the set pointer to the vector passed back from set->munion()
  Set set = Set("-");
  Set set2 = set.munion(Term());
  return set2;
}

/** Returns the First set of RelationalOperator, which is "<", ">" and "=". */
Set First::RelationalOperator()
{
  Set set = Set("<");
  set.add(">");
  set.add("=");
  return set;
  
}

/** Returns the First set of AddingOperator, which is "+" and "-". */
Set First::AddingOperator()
{
  Set set = Set("+");
  set.add("-");
  return set;
}

/** Returns the First set of Term, which is that of Factor. */
Set First::Term()
{
  return Factor();
}

/** Returns the First set of Factor, which is "(", "~" and that of VariableAccess and Constant. */
Set First::Factor()
{
  Set set = Set("(");
  set.add("~");
  return set.munion(VariableAccess()).munion(Constant());
}

/** Returns the First set of MultiplyOperator, which is "*", "\" and "/". */
Set First::MultiplyingOperator()
{
  Set set = Set("*");
  set.add("/");
  set.add("\\");
  return set;
}

/** Returns the First set of IndexedSelector, which is "[". */
Set First::IndexedSelector()
{
  return Set("[");
}

/** Returns the First set of Numeral, which is "num". */
Set First::Numeral()
{
  //First::Numeral() == {"num"}
  return Set("num");
}

/** Returns the First set of BooleanSymbol, which is "true" and "false". */
Set First::BooleanSymbol()
{
  Set set = Set("true");
  set.add("false");
  
  return set;
}

/** Returns the First set of ConstantName, which is "name". */
Set First::ConstantName()
{
  Set set = Set("name");
  return set;
}

/////////////////////////////////////////////////

/** Returns the First set of ConstantDefinition, which is "const". */
Set First::ConstantDefinition() 
{
  Set set = Set("const");
  return set;	
}

/** Returns the First set of VariableDefinition, which is that of TypeSymbol. */
Set First::VariableDefinition() 
{
  return TypeSymbol();	
}

/** Returns the First set of VariableDefinitionPart, which is "array" and that of VariableList. */
Set First::VariableDefinitionPart() 
{
  Set set = Set("array");
  return set.munion(VariableList());	
}

/** Returns the First set of TypeSymbol, which is "integer" and "boolean". */
Set First::TypeSymbol() 
{
  Set set = Set("integer");
  set.add("Boolean");

  return set;	
}

/** Returns the First set of VariableList, which is that of VariableName. */
Set First::VariableList() 
{
  return VariableName();	
}

/** Returns the First set of ProcedureDefinition, which is "proc". */
Set First::ProcedureDefinition() 
{
  Set set = Set("proc");
  return set;	
}

/** Returns the First set of VariableName, which is "name" */
Set First::VariableName() 
{
  Set set = Set("name");
  return set;	
}

/** Returns the First set of FactorName, which is "name". */
Set First::FactorName() 
{
  return Set("name");	
}