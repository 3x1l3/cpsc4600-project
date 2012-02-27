#include "first.h"

First::First() {

	//Something	
	//nothing
	
}

Set First::Program() {
	return Block();	
}

Set First::Block() {
	return Set("begin");	
}

Set First::DefinitionPart() {
	Set set = Set("");
	Set set2 = set.munion(Definition());
	return set2;
}

Set First::Definition() {
	return ConstantDefinition().munion(VariableDefinition()).munion(ProcedureDefinition());
}

///////////////////////////////////////////////////
////Prototypical Functions Start
/// I don't know what we want or need in these functions. But MAKE now works.

Set First::StatementPart()
{
  Set set = Set("");
  Set set2 = set.munion(Statement());  //TODO does this actually change set anymore? casue we made it so it didnt change the oroiginal set
  //so maybe this is not working right anymore, as sets original set is no changed, but rather a ducpliate variable is changed.
  //Should work now.
  return set2;
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
  return Set("name");
}
Set First::EmptyStatement()
{
  return Set("skip");
}
Set First::ReadStatement()
{
  return Set("read");
}
Set First::WriteStatement()
{
  return Set("write");
}
Set First::AssignmentStatement()
{
  return VariableAccessList();
}
Set First::ProcedureStatement()
{
  return Set("call");
}
Set First::IfStatement()
{
  return Set("if");
}
Set First::DoStatement()
{
  return Set("do");
}
Set First::VariableAccessList()
{
  return VariableAccess();
  
}

Set First::VariableAccess()
{
  return VariableName().munion(IndexedSelector());
  
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
  Set set = Set("&");
  set.add("|");
  return set;
  
}

Set First::SimpleExpression()
{
  //TODO this may not be right.  setting the set pointer to the vector passed back from set->munion()
  Set set = Set("-");
  Set set2 = set.munion(Term());
  return set2;
  
}

Set First::RelationalOperator()
{
  Set set = Set("<");
  set.add(">");
  set.add("=");
  return set;
  
}
Set First::AddingOperator()
{
  Set set = Set("+");
  set.add("-");
  return set;
  
}
Set First::Term()
{
  return Factor();
  
}
Set First::Factor()
{
  Set set = Set("(");
  set.add("~");
  return set.munion(VariableAccess()).munion(Constant());
  
}
Set First::MultiplyingOperator()
{
  Set set = Set("*");
  set.add("/");
  set.add("\\");
  return set;
  
}
Set First::IndexedSelector()
{
  return Set("[");
  
}
Set First::Numeral()
{
  //First::Numeral() == {"num"}
  return Set("num");
  
}
Set First::BooleanSymbol()
{
  Set set = Set("true");
  set.add("false");
  
  return set;
  
}


Set First::ConstantName()
{
  Set set = Set("name");
  return set;
}

/////////////////////////////////////////////////


Set First::ConstantDefinition() {
	Set set = Set("const");
	return set;	
}

Set First::VariableDefinition() {
	return TypeSymbol();	
}

Set First::VariableDefinitionPart() {
	Set set = Set("array");
	return set.munion(VariableList());	
}

Set First::TypeSymbol() {
	
	Set set = Set("integer");
	set.add("Boolean");
	
	return set;	
}

Set First::VariableList() {
	return VariableName();	
}

Set First::ProcedureDefinition() {
	Set set = Set("proc");
	return set;	
}

Set First::VariableName() {
	Set set = Set("name");
	return set;	
}

Set First::FactorName() {
	return Set("name");	
}