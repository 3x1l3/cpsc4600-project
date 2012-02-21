#include "first.h"

First::First() {

	//Something	
	
}

Set First::Program() {
	return Block();	
}

Set First::Block() {
	Set* set = new Set();
	set->add("begin");
	return *set;	
}

Set First::DefinitionPart() {
	
	Set* set = new Set();
	set->add("");
	set->munion(Definition());
	return *set;
		
}

Set First::Definition() {
	return ConstantDefinition().munion(VariableDefinition()).munion(ProcedureDefinition());
}

///////////////////////////////////////////////////
////Prototypical Functions Start
/// I don't know what we want or need in these functions. But MAKE now works.

Set First::StatementPart()
{
  return *new Set();
}

Set First::Statement()
{
  return *new Set();
}

Set First::Constant()
{
  return *new Set();
}

Set First::ProcedureName()
{return *new Set();}
Set First::EmptyStatement()
{return *new Set();
}
Set First::ReadStatement()
{return *new Set();
}
Set First::WriteStatement()
{return *new Set();
}
Set First::AssignmentStatement()
{return *new Set();
}
Set First::ProcedureStatement()
{return *new Set();
}
Set First::IfStatement()
{return *new Set();
}
Set First::DoStatement()
{return *new Set();
}
Set First::VariableAccessList()
{return *new Set();}

Set First::VariableAccess()
{return *new Set();}

Set First::ExpressionList()
{return *new Set();}

Set First::Expression()
{return *new Set();}


Set First::GuardedCommandList()
{return *new Set();}

Set First::GuardedCommand()
{return *new Set();}

Set First::PrimaryExpression()
{return *new Set();}

Set First::PrimaryOperator()
{return *new Set();}

Set First::SimpleExpression()
{return *new Set();}

Set First::RelationalOperator()
{return *new Set();}
Set First::AddingOperator()
{return *new Set();}
Set First::Term()
{return *new Set();}
Set First::Factor()
{return *new Set();}
Set First::MultiplyingOperator()
{return *new Set();}


//redundant?
Set First::ConstantName()
{
  return ConstantDefinition();
}

/////////////////////////////////////////////////


Set First::ConstantDefinition() {
	Set* set = new Set("const");
	return *set;	
}

Set First::VariableDefinition() {
	return TypeSymbol();	
}

Set First::VariableDefinitionPart() {
	Set* set = new Set("array");
	return set->munion(VariableList());	
}

Set First::TypeSymbol() {
	Set* set = new Set("integer");
	set->add("Boolean");
	return *set;	
}

Set First::VariableList() {
	return VariableName();	
}

Set First::ProcedureDefinition() {
	Set* set = new Set("proc");
	return *set;	
}

Set First::VariableName() {
	Set* set = new Set("ID");
	return *set;	
}