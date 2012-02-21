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
  
}

Set First::Statement()
{
  
}

Set First::Constant()
{
  
}

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