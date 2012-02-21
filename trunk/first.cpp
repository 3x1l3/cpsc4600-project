#include "first.h"

First::First() {

	//Something	
	//nothing
	
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
  Set* set = new Set();
  set->add("");
  set->munion(Statement());  //TODO does this actually change set anymore? casue we made it so it didnt change the oroiginal set
  //so maybe this is not working right anymore, as sets original set is no changed, but rather a ducpliate variable is changed.
  return *set;
}

Set First::Statement()
{
  return EmptyStatement().munion(ReadStatement()).munion(WriteStatement()).munion(AssignmentStatement()).munion(ProcedureStatement())
  .munion(IfStatement()).munion(DoStatement());
}

Set First::Constant()
{
  return Numeral().munion(BooleanSymbol()).munion(ConstantName());
}

Set First::ProcedureName()
{
  Set *set = new Set("name");
  return *set;
  
}
Set First::EmptyStatement()
{
  Set* set = new Set("skip");
  return *set;
}
Set First::ReadStatement()
{
  Set* set = new Set("read");
  return *set;
}
Set First::WriteStatement()
{
  Set * set = new Set("write");
  return *set;
}
Set First::AssignmentStatement()
{
  return VariableAccessList();
}
Set First::ProcedureStatement()
{
  Set * set = new Set("call");
  return *set;
}
Set First::IfStatement()
{
  return *new Set("if");
}
Set First::DoStatement()
{
  return *new Set("do");
}
Set First::VariableAccessList()
{
  return VariableAccess();
  
}

Set First::VariableAccess()
{
  return VariableName();
  
}

Set First::ExpressionList()
{
  return Expression();
  
}

Set First::Expression()
{
  return PrimaryExpression();
  
}


Set First::GuardedCommandList()
{
  return GuardedCommand();
  
}

Set First::GuardedCommand()
{
  return Expression();
  
}

Set First::PrimaryExpression()
{
  return SimpleExpression();
  
}

Set First::PrimaryOperator()
{
  Set* set = new Set("&");
  set->add("|");
  return *set;
  
}

Set First::SimpleExpression()
{
  //TODO this may not be right.  setting the set pointer to the vector passed back from set->munion()
  Set* set = new Set("-");
  *set = set->munion(Term());
  return *set;
  
}

Set First::RelationalOperator()
{
  Set* set = new Set("<");
  set->add(">");
  set->add("=");
  return *set;
  
}
Set First::AddingOperator()
{
  Set* set = new Set("+");
  set->add("-");
  return *set;
  
}
Set First::Term()
{
  return Factor();
  
}
Set First::Factor()
{
  Set *set = new Set("(");
  set->add("~");
  return Constant.munion(VariableAccess()).munion(*set);
  
}
Set First::MultiplyingOperator()
{
  Set* set = new Set("*");
  set->add("/");
  set->add("\\");
  return *set;
  
}
Set First::IndexedSelector()
{
  return *new Set("[");
  
}
Set First::Numeral()
{
  //First::Numeral() == {"num"}
  return *new Set("num");
  
}
Set First::BooleanSymbol()
{
  Set* set = new Set("true");
  set->add("false");
  
  return *set;
  
}


Set First::ConstantName()
{
  Set *set = new Set("name");
  return *set;
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
	Set* set = new Set("name");
	return *set;	
}